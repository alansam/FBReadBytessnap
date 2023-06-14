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
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cctype>

using namespace std::literals::string_literals;

typedef uint8_t octet;

extern "C"
int snap(FILE * fp, void const * op, size_t op_l);
void make_data(void);
void copy_data(void);
void check_data(void);
void string_it(void);


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

auto const ifile = "jinkie"s;
auto const ofile = "junkie"s;

/*
 *  MARK:  main()
 */
int main(int argc, char const * argv[]) {
  std::cout << "In function "s
            << __func__ << "()\n"s;

  int RC { EXIT_SUCCESS };

  std::cout << std::string(40, '.') << '\n';
  snap(stdout, indata.data(), indata.size());

  try {
    std::cout << std::string(40, '.') << '\n';
    make_data();

    std::cout << std::string(40, '.') << '\n';
    copy_data();

    std::cout << std::string(40, '.') << '\n';
    check_data();

    std::cout << std::string(40, '.') << '\n';
    string_it();
  }
  catch (std::exception & ex) {
    std::cout << "GRONK!: "s << ex.what() << '\n';
    RC = EXIT_FAILURE;
  }

  return RC;
}

/*
 *  MARK:  copy_data()
 */
void copy_data(void) {
  std::cout << "In function "s
            << __func__ << "()\n"s;
  uint8_t ch = EOF;
  std::ifstream innie(ifile, std::ios::binary);
  std::ofstream outie(ofile, std::ios::binary);

  if (innie.is_open() && outie.is_open()) {
    while (true) {
      ch = innie.get();
      if (innie.eof()) {
        break;
      }
      outie << ch;
    }

    innie.close();
    outie.close();
  }
  else {
    std::ostringstream err;
    if (innie.is_open()) {
      err << "Unable to open target file: "s
          << ifile;
    }
    else {
      err << "Unable to open target file: "s
          << ofile;      
    }

    throw std::runtime_error(err.str());
  }

  return;
}

/*
 *  MARK:  make_data()
 */
void make_data(void) {
  std::cout << "In function "s
            << __func__ << "()\n"s;
  std::ofstream off(ifile, std::ios::binary);
  if (off.is_open()) {
    for (auto ch : indata) {
      off << ch;
    }

    off.close();
  }
  else {
    std::ostringstream err;
    err << "Unable to open target file: "s
        << ifile; 
    throw std::runtime_error(err.str());
  }

  return;
}

/*
 *  MARK:  check_data()
 */
void check_data(void) {
  std::cout << "In function "s
            << __func__ << "()\n"s;
  std::ifstream iff(ofile, std::ios::binary);
  if (iff.is_open()) {
    std::vector<uint8_t> chex;
    uint8_t ch = EOF;
    while (true) {
      ch = iff.get();
      if (iff.eof()) {
        break;
      }
      chex.push_back(ch);
    }
    iff.close();

    snap(stdout, chex.data(), chex.size());
  }
  else {
    std::ostringstream err;
    err << "Unable to open target file: "s
        << ofile; 
    throw std::runtime_error(err.str());    
  }

  return;
}

/*
 *  MARK:  string_it()
 */
void string_it(void) {
  std::cout << "In function "s
            << __func__ << "()\n"s;
  std::stringstream id(
      std::ios_base::in
    | std::ios_base::out
    | std::ios_base::binary);
  for (auto ch : indata) {
    id.put(ch);
  }

  snap(stdout, id.str().data(), id.str().size());

  std::ostringstream tgt(
      std::ios_base::out
    | std::ios_base::binary);

  uint8_t ch = EOF;
  while (true) {
    ch = id.get();
    if (id.eof()) {
      break;
    }
    tgt << ch;
  }
  
  snap(stdout, tgt.str().data(), tgt.str().size());

  return;
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
