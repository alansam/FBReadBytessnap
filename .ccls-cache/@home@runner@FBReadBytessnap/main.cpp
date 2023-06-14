/*
Hi everyone,

Just need help for below code to open the file and copy the data, unfortunately the values are not matching,

#include <fstream>
#include <iostream>
 
int main(void)
{
 char ch;
    
 std::ifstream fin("C:\\123x.pgm");
 std::ofstream fout("C:\\output.pgm");
 
    while (!fin.eof())
    {
    ch=fin.get();
 fout<<ch;
    }
    return 0;
}

may the space character or new line character is copying to file. 

Any solution?
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cctype>

typedef uint8_t octet;

extern "C"
int snap(FILE * fp, void const * op, size_t op_l);

static
std::vector<octet> const indata {
  0x50, 0x36, 0x0d, 0x0a,
  0x32, 0x0d, 0x0a,
  0x31, 0x0d, 0x0a,
  0x32, 0x35, 0x35, 0x0d, 0x0a,
  0x00,
  0xff, 0xff, 0xff,
  0x00,
  0xff, 0xff,
};

/*
 *  MARK:  main()
 */
int main(int argc, char const * argv[]) {
  std::cout << "Hello World\n";
  snap(stdout, indata.data(), indata.size());

  return 0;
}

//
//  snap.c
//  CF.snap
//
//  Created by Alan Sampson on 6/22/20.
//  Copyright © 2020 Alan @ FreeShell. All rights reserved.
//

//  MARK: - Implementation.
/*
 *  MARK: snap()
 *  SVC 51 (0A33)
 */
extern "C"
int snap(FILE * fp, void const * opvar, size_t op_l) {

  octet const * op = (octet const *) opvar;
  int chr_ct = 0;

  size_t const LL = 0x20; //  snap line length (32)
  size_t const GP = 0x08; //  distance between print gaps
  octet const * op_end = op + op_l;
  octet const * op_now = op;

  chr_ct += fprintf(fp, "%p:\n", (void *) op);
  while (op_now < op_end) {
    octet const * hex_p = op_now;
    octet const * chr_p = op_now;

    chr_ct += fprintf(fp, "%04" PRIxPTR ": ", hex_p - op);
    for (size_t xc = 0, sp = 1; xc < LL; ++xc) {
      octet const * loc = hex_p++;
      if (loc < op_end) {
        octet x_ = *loc;
        chr_ct += fprintf(fp, "%02" PRIx8 "%s", x_, (sp++ % GP == 0) ? " " : "");
      }
      else {
        chr_ct += fprintf(fp, "  %s", (sp++ % GP == 0) ? " " : "");
      }
    }
    chr_ct += fprintf(fp, " ");

    for (size_t cc = 0, sp = 1; cc < LL; ++cc) {
      octet const * loc = chr_p++;
      octet c_ = loc < op_end ? *loc : ' ';
      chr_ct += fprintf(fp, "%c%s", isprint(c_) ? c_ : '.', (sp++ % GP == 0) ? " " : "");
    }
    chr_ct += fprintf(fp, "\n");
    op_now += LL;
  }
  chr_ct += fprintf(fp, "\n");

  return chr_ct;
}
