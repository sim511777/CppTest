#include <iostream>
#include <string>

int main() {
    // string class usage examples
    std::string s1 = "Hello";
    std::string s2 = "World";
    std::string s3 = s1 + " " + s2 + "!";
    std::cout << s3 << std::endl;
    std::cout << s3.length() << std::endl;
    std::cout << s3[0] << std::endl;
    std::cout << s3[1] << std::endl;
    // copy constructor
    std::string s4 = s3;
    std::cout << s4 << std::endl;
    // assignment operator
    s4 = s1;
    std::cout << s4 << std::endl;
    // move constructor
    std::string s5 = std::move(s4);
    std::cout << s5 << std::endl;
    // move assignment operator
    s4 = std::move(s5);
    std::cout << s4 << std::endl;
    std::cout << s5 << std::endl;
    // append
    s4.append(" ");
    s4.append(s5);
    std::cout << s4 << std::endl;
    // insert
    s4.insert(5, " ");
    std::cout << s4 << std::endl;
    // erase
    s4.erase(5, 1);
    std::cout << s4 << std::endl;
    // replace
    s4.replace(5, 1, " ");
    std::cout << s4 << std::endl;
    // find
    std::cout << s4.find("World") << std::endl;
    std::cout << s4.find("World", 7) << std::endl;
    std::cout << s4.find("World", 8) << std::endl;
    // rfind
    std::cout << s4.rfind("World") << std::endl;
    std::cout << s4.rfind("World", 7) << std::endl;
    std::cout << s4.rfind("World", 8) << std::endl;
    // find_first_of
    std::cout << s4.find_first_of("World") << std::endl;
    std::cout << s4.find_first_of("World", 7) << std::endl;
    std::cout << s4.find_first_of("World", 8) << std::endl;
    // find_last_of
    std::cout << s4.find_last_of("World") << std::endl;
    std::cout << s4.find_last_of("World", 7) << std::endl;
    std::cout << s4.find_last_of("World", 8) << std::endl;
    // find_first_not_of
    std::cout << s4.find_first_not_of("Hello") << std::endl;
    std::cout << s4.find_first_not_of("Hello", 7) << std::endl;
    std::cout << s4.find_first_not_of("Hello", 8) << std::endl;
    // find_last_not_of
    std::cout << s4.find_last_not_of("Hello") << std::endl;
    std::cout << s4.find_last_not_of("Hello", 7) << std::endl;
    std::cout << s4.find_last_not_of("Hello", 8) << std::endl;
    // substr
    std::cout << s4.substr(0, 5) << std::endl;
    std::cout << s4.substr(6, 5) << std::endl;
    // compare
    std::cout << s1.compare(s2) << std::endl;
    std::cout << s1.compare(0, 5, s2) << std::endl;
    std::cout << s1.compare(0, 5, s2, 0, 5) << std::endl;
    // c_str
    std::cout << s4.c_str() << std::endl;
    // data
    std::cout << s4.data() << std::endl;
    // empty
    std::cout << s4.empty() << std::endl;
    // clear
    s4.clear();
    std::cout << s4.empty() << std::endl;
    // resize
    s4.resize(5);
    std::cout << s4 << std::endl;
    s4.resize(10);
    std::cout << s4 << std::endl;
    s4.resize(15, '!');
    std::cout << s4 << std::endl;
    // swap
    s4.swap(s5);
    std::cout << s4 << std::endl;
    std::cout << s5 << std::endl;
    // push_back
    s4.push_back('!');
    std::cout << s4 << std::endl;
    // pop_back
    s4.pop_back();
    std::cout << s4 << std::endl;
    // begin
    std::cout << *s4.begin() << std::endl;
    // end
    std::cout << *(s4.end() - 1) << std::endl;
    // rbegin
    std::cout << *s4.rbegin() << std::endl;
    // rend

    
    
    // comparison operators
    std::cout << (s1 == s2) << std::endl;
    std::cout << (s1 != s2) << std::endl;
    std::cout << (s1 < s2) << std::endl;
    std::cout << (s1 > s2) << std::endl;
    std::cout << (s1 <= s2) << std::endl;
    std::cout << (s1 >= s2) << std::endl;
    // substring
    std::cout << s3.substr(0, 5) << std::endl;
    std::cout << s3.substr(6, 5) << std::endl;
    // find
    std::cout << s3.find("World") << std::endl;
    std::cout << s3.find("World", 7) << std::endl;
    std::cout << s3.find("World", 8) << std::endl;
    // rfind
    std::cout << s3.rfind("World") << std::endl;
    std::cout << s3.rfind("World", 7) << std::endl;
    std::cout << s3.rfind("World", 8) << std::endl;
    // find_first_of
    std::cout << s3.find_first_of("World") << std::endl;
    std::cout << s3.find_first_of("World", 7) << std::endl;
    std::cout << s3.find_first_of("World", 8) << std::endl;
    // find_last_of
    std::cout << s3.find_last_of("World") << std::endl;
    std::cout << s3.find_last_of("World", 7) << std::endl;
    std::cout << s3.find_last_of("World", 8) << std::endl;
    // find_first_not_of
    std::cout << s3.find_first_not_of("Hello") << std::endl;
    std::cout << s3.find_first_not_of("Hello", 7) << std::endl;
    std::cout << s3.find_first_not_of("Hello", 8) << std::endl;
    // find_last_not_of
    std::cout << s3.find_last_not_of("Hello") << std::endl;
    std::cout << s3.find_last_not_of("Hello", 7) << std::endl;
    std::cout << s3.find_last_not_of("Hello", 8) << std::endl;
    // substr
    std::cout << s3.substr(0, 5) << std::endl;
    std::cout << s3.substr(6, 5) << std::endl;
    // compare
    std::cout << s1.compare(s2) << std::endl;
    std::cout << s1.compare(0, 5, s2) << std::endl;
    std::cout << s1.compare(0, 5, s2, 0, 5) << std::endl;
    // c_str
    std::cout << s3.c_str() << std::endl;
    // data
    std::cout << s3.data() << std::endl;
    // empty
    std::cout << s3.empty() << std::endl;
    // clear
    s3.clear();
    std::cout << s3.empty() << std::endl;
    // resize
    s3.resize(5);
    std::cout << s3 << std::endl;
    s3.resize(10);
    std::cout << s3 << std::endl;
    s3.resize(15, '!');
    std::cout << s3 << std::endl;
    // swap
    s3.swap(s4);
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl;
    // push_back
    s3.push_back('!');
    std::cout << s3 << std::endl;
    // pop_back
    s3.pop_back();
    std::cout << s3 << std::endl;
    // begin
    std::cout << *s3.begin() << std::endl;
    // end
    std::cout << *(s3.end() - 1) << std::endl;
    // rbegin
    std::cout << *s3.rbegin() << std::endl;
    // rend
    std::cout << *(s3.rend() - 1) << std::endl;
    // operator[]
    std::cout << s3[0] << std::endl;
    std::cout << s3[1] << std::endl;
    // at
    std::cout << s3.at(0) << std::endl;
    std::cout << s3.at(1) << std::endl;
    // front
    std::cout << s3.front() << std::endl;
    // back
    std::cout << s3.back() << std::endl;
    // data
    std::cout << s3.data() << std::endl;
    // c_str
    std::cout << s3.c_str() << std::endl;
    // append
    s3.append(" ");
    s3.append(s4);
    std::cout << s3 << std::endl;
    // insert
    s3.insert(5, " ");
    std::cout << s3 << std::endl;
    // erase
    s3.erase(5, 1);
    std::cout << s3 << std::endl;
    // replace
    s3.replace(5, 1, " ");
    std::cout << s3 << std::endl;
    // copy
    char buf[100];
    s3.copy(buf, 5, 0);
    std::cout << buf << std::endl;
    // resize
    s3.resize(5);
    std::cout << s3 << std::endl;
    s3.resize(10);
    std::cout << s3 << std::endl;
    s3.resize(15, '!');
    std::cout << s3 << std::endl;
    // swap
    s3.swap(s4);
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl;
    // push_back
    s3.push_back('!');
    std::cout << s3 << std::endl;
    // pop_back
    s3.pop_back();
    std::cout << s3 << std::endl;
    // begin
    std::cout << *s3.begin() << std::endl;
    // end
    std::cout << *(s3.end() - 1) << std::endl;
    // rbegin
    std::cout << *s3.rbegin() << std::endl;
    // rend
    std::cout << *(s3.rend() - 1) << std::endl;
    // operator[]
    std::cout << s3[0] << std::endl;
    std::cout << s3[1] << std::endl;
    // at
    std::cout << s3.at(0) << std::endl;
    std::cout << s3.at(1) << std::endl;
    // front
    std::cout << s3.front() << std::endl;
    // back
    std::cout << s3.back() << std::endl;
    // data
    std::cout << s3.data() << std::endl;
    // c_str
    std::cout << s3.c_str() << std::endl;
    
    // string contains a string or a character
    std::cout << s3.find("World") << std::endl;
    std::cout << s3.find("World", 7) << std::endl;
    std::cout << s3.find("World", 8) << std::endl;
    std::cout << s3.rfind("World") << std::endl;
    std::cout << s3.rfind("World", 7) << std::endl;
    std::cout << s3.rfind("World", 8) << std::endl;
    std::cout << s3.find_first_of("World") << std::endl;
    std::cout << s3.find_first_of("World", 7) << std::endl;
    std::cout << s3.find_first_of("World", 8) << std::endl;
    std::cout << s3.find_last_of("World") << std::endl;
    std::cout << s3.find_last_of("World", 7) << std::endl;
    std::cout << s3.find_last_of("World", 8) << std::endl;
    std::cout << s3.find_first_not_of("Hello") << std::endl;
    std::cout << s3.find_first_not_of("Hello", 7) << std::endl;
    std::cout << s3.find_first_not_of("Hello", 8) << std::endl;
    std::cout << s3.find_last_not_of("Hello") << std::endl;
    std::cout << s3.find_last_not_of("Hello", 7) << std::endl;
    std::cout << s3.find_last_not_of("Hello", 8) << std::endl;
    std::cout << s3.substr(0, 5) << std::endl;
    std::cout << s3.substr(6, 5) << std::endl;
    std::cout << s3.compare(s4) << std::endl;
    std::cout << s3.compare(0, 5, s4) << std::endl;
    std::cout << s3.compare(0, 5, s4, 0, 5) << std::endl;
    std::cout << s3.c_str() << std::endl;
    std::cout << s3.data() << std::endl;
    std::cout << s3.empty() << std::endl;
    s3.clear();
    std::cout << s3.empty() << std::endl;
    s3.resize(5);
    std::cout << s3 << std::endl;






    

    

    std::cout << "Hello World!\n";
}

