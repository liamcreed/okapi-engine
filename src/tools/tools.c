#include "tools/tools.h"

void write_data_to_binary_file(unsigned char* buffer, const char* file_path)
{

}

char *get_full_path_from_other(const char *path, char *file_name)
{
    u32 path_length = strlen(path);
    u32 l;

    for (u32 i = 0; i < path_length; i++)
    {
        if (path[path_length - i] == '/')
        {
            l = i;
            break;
        }
    }
    char *directory = malloc(sizeof(char) * 100);
    for (u32 i = 0; i < path_length - l + 1; i++)
    {
        directory[i] = path[i];
    }
    directory[path_length - l + 1] = '\0';
    strcat(directory, file_name);
    return directory;
}