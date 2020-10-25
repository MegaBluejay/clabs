#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BUFFER_SIZE = 100;
const int MAX_FRAMES = 100;

void fread_reversed(void* ogbuffer, size_t size, size_t count, FILE* stream) {
  char* buffer = (char*) ogbuffer;
  fread(buffer, size, count, stream);
  for (int i=0; i<count; i++) {
    for (int j=0; j<size/2; j++) {
      char temp = buffer[i*size+j];
      buffer[i*size+j] = buffer[(i+1)*size-1-j];
      buffer[(i+1)*size-1-j] = temp;
    }
  }
}

int read_props(char output[MAX_FRAMES*2][BUFFER_SIZE], FILE* stream) {
  // output should be an array of at least n*2 initialized strings
  // where n is the number of text frames in the tag
  // stream should be in rb mode
  char buffer[BUFFER_SIZE];

  // check id3v2 signature
  fread(buffer, 1, 3, stream);
  if (strncmp(buffer, "ID3", 3)) {
    return 0;
  }

  // skip version and flags
  fseek(stream, 3, SEEK_CUR);

  // read tag size
  int tag_size = 0;
  fread_reversed(buffer, 4, 1, stream);
  for (int i=0;i<4;i++) {
    tag_size += buffer[i]<<(7*(3-i));
  }

  int frame_number = 0;
  while (ftell(stream)-10 < tag_size && frame_number<MAX_FRAMES) {
    // read frame id
    fread(buffer, 1, 4, stream);
    buffer[4] = '\0';

    // assumming all frames are grouped at the beginning
    if (!strcmp(buffer, "\0\0\0\0")) {
      break;
    }
    
    // read frame size
    int frame_size;
    fread_reversed(&frame_size, 4, 1, stream);

    // skip flags
    fseek(stream, 2, SEEK_CUR);

    // only parse text frames
    if (buffer[0] == 'T') {
      // write frame name to output
      strcpy(output[frame_number*2], buffer);
      
      // skip encoding
      fseek(stream, 1, SEEK_CUR);

      // read frame value directly to output
      fread(output[frame_number*2+1], 1, frame_size-1, stream);
      output[frame_number*2+1][frame_size-1] = '\0';

      frame_number++;
    }
    else {
      fseek(stream, frame_size, SEEK_CUR);
    }
  }
  
  // return total number of frames written to output
  return frame_number;
}

int get_prop(char* buffer, char props[MAX_FRAMES*2][BUFFER_SIZE], int n_frames, char* prop_name) {
  for (int i=0; i<n_frames; i++) {
    if (!strcmp(props[i*2], prop_name)) {
      strcpy(buffer, props[i*2+1]);
      return 0;
    }
  }
  return 1;
}

int set_prop(char props[MAX_FRAMES*2][BUFFER_SIZE], int n_frames, char* prop_name, char* prop_value) {
  for (int i=0; i<n_frames; i++) {
    if (!strcmp(props[i*2], prop_name)) {
      strcpy(props[i*2+1], prop_value);
      return 0;
    }
  }
  strcpy(props[n_frames*2], prop_name);
  strcpy(props[n_frames*2+1], prop_value);
  return 1;
}

void write_props(char input[MAX_FRAMES*2][BUFFER_SIZE], int n_frames, FILE* stream) {
  // stream should be in rb+
  char buffer[BUFFER_SIZE];

  int current_tag_size = 0;
  fread(buffer, 1, 3, stream);
  if (!strncmp(buffer, "ID3", 3)) {
    fseek(stream, 3, SEEK_CUR);
    fread(buffer, 1, 4, stream);
    for (int i=0; i<4; i++) {
      current_tag_size += buffer[i]<<(7*(3-i));
    }
    current_tag_size+=10;
  }

  int new_tag_size = 10;
  for (int i=0; i<n_frames; i++) {
    new_tag_size+=10+strlen(input[i*2+1]);
  }

  // only change the file size if we need to
  if (new_tag_size>current_tag_size) {
    // get total file size and calculate mp3 size
    fseek(stream, 0, SEEK_END);
    int mp3_size = ftell(stream)-current_tag_size;

    // read mp3 data into memory
    fseek(stream, current_tag_size, SEEK_SET);
    char* mp3 = malloc(mp3_size);
    fread(mp3, 1, mp3_size, stream);

    //write it back moved to where it needs to go and release memory
    fseek(stream, new_tag_size, SEEK_SET);
    fwrite(mp3, 1, mp3_size, stream);
    free(mp3);

    // write new tag header ;todo test this
    fseek(stream, 0, SEEK_SET);
    char new_header[6];
    strncpy(new_header, "ID3", 3);
    new_header[3] = 3;
    fwrite(new_header, 1, 6, stream);
    int size_calc = new_tag_size<<4;
    int mask = ((1<<7)-1)<<25;
    for (int i=0; i<4; i++) {
      fputc(size_calc & mask, stream);
      size_calc = size_calc << 7;
    }
  }
  else {
    fseek(stream, new_tag_size, SEEK_SET);
    if (new_tag_size<current_tag_size-4) {
      for (int i=0; i<4; i++) {
        fputc(0,stream);
      }
    }
    else {
      for (int i=0; i<current_tag_size-new_tag_size; i++) {
        fputc(0, stream);
      }
    }
    fseek(stream, 10, SEEK_SET);
  }
  for (int i=0; i<n_frames; i++) {
    fwrite(input[i*2], 1, 4, stream);
    int frame_size = strlen(input[i*2+1])+1;
    int size_calc = frame_size;
    for (int i=0; i<4; i++) {
      fputc(size_calc>>24, stream);
      size_calc = size_calc<<8;
    }
    for (int i=0; i<3; i++) {
      fputc(0, stream);
    }
    fwrite(input[i*2+1], 1, frame_size-1, stream);
  }
}

int main(int argc, char *argv[]) {
  int show=0, set=0, get=0;
  char *filepath, *prop_name, *prop_value;
  for (int i=1; i<argc; i++) {
    char *arg_name;
    arg_name = strtok(&(argv[i][2]), "=");
    if (!strcmp(arg_name, "filepath")) {
      filepath = strtok(NULL, "=");
    }
    else if (!strcmp(arg_name, "show")) {
      show = 1;
    }
    else if (!strcmp(arg_name, "set")) {
      set = 1;
      prop_name = strtok(NULL, "=");
    }
    else if (!strcmp(arg_name, "value")) {
      prop_value = strtok(NULL, "=");
    }
    else if (!strcmp(arg_name, "get")) {
      get = 1;
      prop_name = strtok(NULL, "=");
    }
  }
  
  if (filepath) {
    FILE* file =  fopen(filepath, "rb+");
    char props[MAX_FRAMES*2][BUFFER_SIZE];
    int n = read_props(props, file);
    if (show) {
      for (int i=0; i<n; i++) {
        printf("%s=%s\n", props[i*2], props[i*2+1]);
      }
    }
    else if (get) {
      if (prop_name) {
        char buffer[BUFFER_SIZE];
        if (!get_prop(buffer, props, n, prop_name)) {
          printf("%s\n", buffer);
        }
        else {
          printf("no such prop");
        }
      }
      else {
        printf("no prop name given\n");
      }
    }
    else if (set) {
      if (prop_name && prop_value) {
        fseek(file, 0, SEEK_SET);
        n += set_prop(props, n, prop_name, prop_value);
        for (int i=0; i<n; i++) {
          printf("%s=%s\n", props[i*2], props[i*2+1]);
        }
        write_props(props, n, file);
      }
      else {
        if (!prop_name) {
          printf("no prop name given\n");
        }
        if (!prop_value) {
          printf("no prop value given\n");
        }
      }
    }
    else {
      printf("invalid arguements\n");
    }
    fclose(file);
  }
  else {
    printf("no filepath given\n");
  }
}
