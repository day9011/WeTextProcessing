// Copyright (c) 2022 Zhendong Peng (pzd17@tsinghua.org.cn)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PROCESSOR_TOKEN_PARSER_H_
#define PROCESSOR_TOKEN_PARSER_H_

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace wenet {

extern const std::string EOS;
extern const std::set<std::string> UTF8_WHITESPACE;
extern const std::set<std::string> ASCII_LETTERS;
extern const std::unordered_map<
  std::string, std::vector<std::string> > TN_ORDERS;
extern const std::unordered_map<
  std::string, std::vector<std::string> > ITN_ORDERS;

struct Token {
  std::string name;
  std::vector<std::string> order;
  std::unordered_map<std::string, std::string> members;

  Token(const std::string& name) : name(name) {}

  void append(const std::string& key, const std::string& value) {
    order.emplace_back(key);
    members[key] = value;
  }

  std::string string(
    const std::unordered_map<std::string, std::vector<std::string>>& orders) {
    std::string output = name + " {";
    if (orders.count(name) > 0) {
      order = orders.at(name);
    }

    for (const auto& key : order) {
      if (members.count(key) == 0) {
        continue;
      }
      output += " " + key + ": \"" + members[key] + "\"";
    }
    return output + " }";
  }
};

class TokenParser {
 public:
  TokenParser(const std::string& far_path);
  std::string reorder(const std::string& input);

 private:
  void load(const std::string& input);
  bool read();
  bool parse_ws();
  bool parse_char(const std::string& exp);
  bool parse_chars(const std::string& exp);
  std::string parse_key();
  std::string parse_value();
  void parse(const std::string& input);

  int index;
  std::string ch;
  std::vector<std::string> text;
  std::vector<Token> tokens;
  std::unordered_map<std::string, std::vector<std::string>> orders;
};

}  // namespace wenet

#endif  // PROCESSOR_TOKEN_PARSER_H_
