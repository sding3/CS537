#include <unistd.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

void
usage(char *prog) 
{
    fprintf(stderr, "usage: %s <-s random seed> <-n number of records> <-o output file>\n", prog);
    exit(1);
}

void 
genstr(string& str, const unordered_set<string>& dup) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

  while (dup.find(str) != dup.end()) {
    for (unsigned i = 0; i < str.size(); i++) {
      str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
  }

  return;
}

int
main(int argc, char* argv[]) {
  // arguments
  int randomSeed  = 0;
  int recordsLeft = 0;

  const char *in = "in";
  const char *out = "out";

  // input params
  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "n:s:")) != -1) {
    switch (c) {
    case 'n':
      recordsLeft = atoi(optarg);
      break;
    case 's':
      randomSeed  = atoi(optarg);
      break;
    default:
      usage(argv[0]);
    }
  }

  // seed random number generator
  srand(randomSeed);

  const unsigned line_len = 126;
  const unsigned word_len = 10;
  string word(word_len, '0');
  string echo_prefix = "echo ";
  string shell_prefix = "whoosh> ";

  unordered_set<string> dup;
  ofstream input(in), output(out);
  for (int i = 0; i < recordsLeft; i++) {
    genstr(word, dup);
    input << echo_prefix + word << endl;
    output << shell_prefix + word << endl;
    dup.insert(word);
  }

  input << "exit" << endl;
  output << shell_prefix;
  input.close();
  output.close();

  return 0;
}
