#pragma once
using namespace std;
#include <bits/stdc++.h>
class dynamic_tape
{
private:
vector<char>tape;
int head=0;
int print_start=0;
 void clear();
     static constexpr int numbers_of_expand = 64;

    void load_initial(const string& initial);
    void seek_start();
    void ensure_exists(int index);
    void update_bounds();

    friend class TuringMachine;


public:
 char read() const;
    void write(char c);
    void move_left();
    void move_right();
    void expand_left();
    void expand_right();
  string print() const;
    int get_head_position_bounds() const;

};

