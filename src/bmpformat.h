#pragma pack(push, 1)
namespace BMP {
struct Header {
    char identifier[0x02] = {0x42, 0x4D};
    int file_size{};
    char more_identifiers[4]{};
    int bitmap_start{};
};
struct DIBHeader {
    int size_of_header = 40;
    int width{};
    int height{};
    unsigned short n_color_panes = 1;
    unsigned short bits_per_pixel = 24;
    int compression = 0;
    int bitmap_size = 0;
    int v_res{};
    int h_res{};
    int n_colors = 0;
    int imp_colors = 0;
};
} // namespace BMP
#pragma pack(pop)
