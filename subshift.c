#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define FPS_RATE 23.976
#define EXT_MAX  4      // assuming a reasonable length for file extensions

int main(int argc, char** argv) {
    
    if (argc != 3) {
        fprintf(stderr, "invalid number of arguments. use:\n"
                "  %s <filepath> <num>\n"
                "to shift subtitles from 'filepath' by 'num' seconds\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filepath = argv[1];
    FILE* src_file = fopen(filepath, "r");
    if (!src_file) {
        fprintf(stderr, "couldn't read file %s\n"
                "usage: %s <filepath> <num>\n", filepath, argv[0]);
        return EXIT_FAILURE;
    }

    const double shift = atof(argv[2]);
    if (shift == 0) {
        fprintf(stderr, "couldn't convert %s to number\n"
                "usage: %s <filepath> <num>\n", argv[2], argv[0]);
        return EXIT_FAILURE;
    }

    const char* suffix = ".shifted.srt";
    int path_len = strlen(filepath) + strlen(suffix) + 1;
    char* output_path = calloc(path_len, sizeof(char));
    if (!output_path) {
        fprintf(stderr, "couldn't allocate %d bytes of memory\n", path_len);
        return EXIT_FAILURE;
    }

    char *pos, *last = (char*)filepath;
    if ((pos = strrchr(filepath, '/'))) {
        last = pos + 1;
    }
    if ((pos = strrchr(last, '.')) && strlen(pos + 1) <= EXT_MAX) {
        last = pos;
    } else {
        last = (char*)filepath + strlen(filepath);
    }
    memcpy(output_path, filepath, last - filepath);
    strcat(output_path, suffix);

    FILE* output_file = fopen(output_path, "w");
    if (!output_file) {
        fprintf(stderr, "couldn't open/create output file %s\n", output_path);
        return EXIT_FAILURE;
    }

    const long long frames = round(shift * FPS_RATE);
    long long start, end;
    int offset = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, src_file)) != -1) {
        if (sscanf(line, "{%lld}{%lld}%n", &start, &end, &offset) != 0) {
            start += frames;
            end += frames;
            fprintf(output_file, "{%lld}{%lld}", start < 0 ? 0 : start, end < 0 ? 0 : end);
            fprintf(output_file, "%s", line + offset);
        }
    }

    printf("subtitles from:\n    %s\nwere shifted by %.1fs "
           "and saved to:\n    %s\n", filepath, shift, output_path);
 
    fclose(src_file);
    fclose(output_file);
    if (line) free(line);
    if (output_path) free(output_path);

}

