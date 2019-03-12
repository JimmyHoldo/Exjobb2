/* port.c */
#include <string.h>
#include <stdio.h>


typedef unsigned char byte;

int main() {
  int fn, arg, res;
  byte buf[100];

  while (read_cmd(buf) > 0) {
    fn = buf[0];
    arg = buf[1];

    if (fn == 1) {
      res = initport(arg);
      buf[0] = res;
      write_cmd(buf, 1);
    }else if (fn == 2) {
      char *ps = buf;
      ps++;

      res = open_port_serial(ps);

      buf[0] = res;
      write_cmd(buf, 1);
    }else if (fn == 3) {
      char content[11];
      res = read_from_zigbee(arg, content);
      //buf[0] = res;
      for (int i = 0; i < strlen(content); i++){
        buf[i] = (byte)(content)[i];
      }
      //write_cmd(buf, 1);
      write_cmd(buf, strlen(content));
    }else if (fn == 4) {
      char *ps = buf;
      ps++;
      ps++;
      res = write_to_zigbee(arg, ps);
      buf[0] = res;
      write_cmd(buf, 1);
    }
  }
}

// FILE *f = fopen("test.txt", "w");
// if (f == NULL)
// {
//     printf("Error opening file!\n");
//     exit(1);
// }
// fprintf(f, "Some text1: %s\n", buf);
// fclose(f);
