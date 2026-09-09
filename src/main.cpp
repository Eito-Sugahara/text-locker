/*
=========================================

=========================================
*/

// Header
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <filesystem>
#include <fstream>
#include "binary.hpp"
#include <algorithm>

// namespace
using namespace std;
namespace fs = std::filesystem;

// Initialize
const vector<vector<string>> scommands = {
  {"help", "help (sub command) - Show a list of sub commands. If the sub command exists, show the sub command's description."},
  {"key-gen", "key-gen <.binac file name> - Generate a key file the file name."},
  {"lock", "lock <txt file name> <.binac file name> - Encrypt and overwrite the txt with the key file."},
  {"ulock", "ulock <txt file name> <.binac file name> - Decrypt and overwrite the txt with the key file."}
};

// Help
int cmd_help() {
  for (int i = 0; i < scommands.size(); i++) {
    cout << scommands[i][1] << endl;
  }
  return 0;
}
int cmd_help(const char* sc) {
  for (int i = 0; i < scommands.size(); i++) {
    if (sc == scommands[i][0]) {
      cout << scommands[i][1] << endl;
      return 0;
    }
  }
  cout << "Error: The sub command \'" << sc << "\' does not exist." << endl;
  return 1;
}
// key-gen
int cmd_keygen(const fs::path& path) {
  // ファイルが存在するなら上書きするか聞く
  if (fs::exists(path)) {
    cout << "File \'" << path << "\' exists." << endl << "Overwrite? [ y / n ]: " << flush;
    string ans;
    cin >> ans;
    if (ans != "y") return 0;
  }
  // text リスト化
  vector<int> abe = {};
  for (int i = 0; i < 256; i++) {
    abe.push_back(i);
  }
  vector<int> aaf(abe);
  // 乱数セットアップ
  random_device rd;
  mt19937 gen(rd());

  // text 分布
  vector<int> acbe = {};
  vector<int> acaf = {};
  for (int i = 0; i < 256; i++) {
    uniform_int_distribution<int> key_dist(0, abe.size() - 1);

    int key = key_dist(gen);
    acbe.push_back(abe[key]);
    abe.erase(abe.begin() + key);

    uniform_int_distribution<int> value_dist(0, aaf.size() - 1);

    int value = value_dist(gen);
    acaf.push_back(aaf[value]);
    aaf.erase(aaf.begin() + value);
  }

  // ファイル出力
  saveKey(acbe, acaf, path);
  return 0;
}

// lock
int cmd_lock(const fs::path& txt_path, const fs::path& key_path) {
  vector<int> ac_key;
  vector<int> ac_value;

  // Load Key data
  loadKey(ac_key, ac_value, key_path);

  // Load txt
  ifstream bfstream(txt_path, ios::binary);
  if (!bfstream) {
    cout << "Error: Failed to open the txt" << endl;
    return 1;
  }
  vector<int> after_ac = {};
  char c;
  while (bfstream.get(c)) {
    int cint = static_cast<unsigned char>(c);
    auto it = find(ac_key.begin(), ac_key.end(), cint);
    int index = static_cast<int>(distance(ac_key.begin(), it));
    after_ac.push_back(ac_value[index]);
  }
  writeEncrypted(after_ac, txt_path);
  return 0;
}

// unlock
int cmd_unlock(const fs::path& txt_path, const fs::path& key_path) {
    // Load locked text
    vector<int> locked_ac = readEncrypted(txt_path);

    // Load key file
    vector<int> ac_key;
    vector<int> ac_value;
    loadKey(ac_key, ac_value, key_path);

    vector<int> unlocked_ac = {};

    for (int i = 0; i < locked_ac.size(); i++) {
        auto it = find(ac_value.begin(), ac_value.end(), locked_ac[i]);

        if (it == ac_value.end()) {
            cout << "Error: Invalid encrypted data" << endl;
            return 1;
        }

        int index = static_cast<int>(distance(ac_value.begin(), it));

        unlocked_ac.push_back(ac_key[index]);
    }

    writeEncrypted(unlocked_ac, txt_path);

    return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return cmd_help();
  }
  else if (argc == 2) {
    return cmd_help(argv[1]);
  }
  else if (argc == 3) {
    if (string(argv[1]) == "help") {
      return cmd_help(argv[2]);
    } else if (string(argv[1]) == "key-gen") {
      return cmd_keygen(argv[2]);
    } else {
      cout << "Error: The command is incomplete" << endl;
      return cmd_help();
    }
  } else if (argc == 4) {
    if (string(argv[1]) == "lock") {
      return cmd_lock(argv[2], argv[3]);
    } else if (string(argv[1]) == "ulock") {
      return cmd_unlock(argv[2], argv[3]);
    } else {
      cout << "Error: The command is incomplete" << endl;
      return cmd_help();
    }
  } else {
    cout << "Error: The command is incomplete" << endl;
    return cmd_help();
  }
}
