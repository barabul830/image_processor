#include "pars_control.h"
Image ReadFromFile(const char *imagefile) {
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;

    FILE *stream = fopen(imagefile, "rb");
    fread(&file_header, sizeof(file_header), 1, stream);
    fread(&info_header, sizeof(info_header), 1, stream);
    fseek(stream, file_header.bfOffBits, SEEK_SET);
    unsigned char *in_rgb = static_cast<unsigned char *>(malloc(file_header.bfSize - file_header.bfOffBits));
    fread(in_rgb, file_header.bfSize - file_header.bfOffBits, 1, stream);
    fclose(stream);
    Image image(in_rgb, file_header, info_header);
    free(in_rgb);
    return image;
}

void WriteToFile(const char *file, Image &image) {
    FILE *outfile = fopen(file, "wb");
    fwrite(&image.FileHeader, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
    fwrite(&image.InfoHeader, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
    fwrite(image.OutRGB(), sizeof(unsigned char), image.FileHeader.bfSize - image.FileHeader.bfOffBits, outfile);
    fclose(outfile);
}
int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "{имя программы} {путь к входному файлу} {путь к выходному файлу}\n"
                     "[-{имя фильтра 1} [параметр фильтра 1] [параметр фильтра 2] ...]\n"
                     "[-{имя фильтра 2} [параметр фильтра 1] [параметр фильтра 2] ...] ...";
    }
    const char *image_file = argv[1];
    const char *out_file = argv[2];
    Image cur_image = ReadFromFile(image_file);
    std::vector<std::vector<std::string>> parsed = ParseAll(argc, argv);
    for (std::vector<std::string> &arg : parsed) {
        try {
            Control(cur_image, arg);
        } catch (std::runtime_error &e) {
            std::cerr << e.what();
        }
    }
    WriteToFile(out_file, cur_image);
}