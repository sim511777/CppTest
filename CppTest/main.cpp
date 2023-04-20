﻿#define _HAS_AUTO_PTR_ETC 1

#include <string>
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <chrono>
#include <thread>
#include <future>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <fstream>
#include <cstdarg>
#include <windows.h>
#include <execution>
#include <ranges>

using namespace std;

int AlignedSize(int size) {
    int ptrSize = sizeof(void*);
    int alignedSize = ((size + (ptrSize - 1)) & ~(ptrSize - 1));
    return alignedSize;
}

// 1. 가변인자에 대한 정보는 다른 인자로 주어져야 한다. : 갯수, 각각의 타입
// 2. 메모리 상에 비가변 인자 다음에 가변 인자들이 순서대로 들어간다.
// 3. 인자들은 붙어서 들어가지 않고 포인터의 크기로 얼라인 되어 들어간다. : 32bit -> 4byte align, 64bit -> 8byte align
int IntSum(int num, ...) {
    char* ap = (char*)&num + AlignedSize(sizeof(num));
    int sum = 0;
    for (int i = 0; i < num; i++) {
        int val = *(int*)ap;
        sum += val;
        ap += AlignedSize(sizeof(int));
    }
    return sum;
}

double DoubleSum(int num, ...) {
    char* ap = (char*)&num + AlignedSize(sizeof(num));
    double sum = 0;
    for (int i = 0; i < num; i++) {
        double val = *(double*)ap;
        sum += val;
        ap += AlignedSize(sizeof(double));
    }
    return sum;
}

wstring StringFormat(wstring fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int size = _vscwprintf(fmt.c_str(), ap) + 1;
    wchar_t* buf = new wchar_t[size];

    vswprintf_s(buf, size, fmt.c_str(), ap);
    wstring str(buf);

    delete[] buf;

    va_end(ap);

    return str;
}

void VariableArgumentTest() {
    cout << "==== VariableArgumentTest ====" << endl << endl;
    int iSum = IntSum(3, 1, 2, 3);
    cout << "int sum = " << iSum << endl;
    double dSum = DoubleSum(3, 1.1, 2.2, 3.3);
    cout << "double sum = " << dSum << endl;
    wstring sSum = StringFormat(L"%d, %f, %s", 123, 123.456, L"hello wstring");
    wcout << "StringFormat = " << sSum << endl;
}

void OpenMPTest() {
    cout << "==== OpenMPTest ====" << endl << endl;
#pragma omp parallel
    {
        cout << "Hello, OpenMP" << endl;
        cout << "Bye, world" << endl;
    }

    int sum = 0;
#pragma omp parallel for
    for (int i = 0; i < 10; i++) {
        cout << "i = " << i << endl;
        sum += i;
    }
    cout << "Sum = " << sum << endl;
}

class Duck {
public:
    void quack() {
        cout << "Quaaaaaack!" << endl;
    }

    void feathers() {
        cout << "The duck has white and gray feathers." << endl;
    }
};

class Person {
public:
    void quack() {
        cout << "The person imitates a duck." << endl;
    }

    void feathers() {
        cout << "The person takes a feather from the ground and shows it." << endl;
    }
};

template <typename T>
void inTheForest(T& t) {
    t.quack();
    t.feathers();
};

void DuckTypingTest() {
    cout << "==== DuckTypingTest ====" << endl << endl;
    Duck donald;
    Person jhon;

    inTheForest(donald);
    inTheForest(jhon);
}

void RangeForTest() {
    cout << "==== RangeForTest ====" << endl << endl;
    // 배열의 경우 로컬에서만 가능
    int arr[5] = { 0, 1, 2, 3, 4, };
    for (int i : arr)
        cout << i << endl;
}

void StringFormatTest() {
    cout << "==== StringFormatTest ====" << endl << endl;
    ostringstream oss;
    oss << " my age : " << 16;
    oss << " my height : " << 170;
    cout << oss.str();
}

void PrintfTest() {
    cout << "==== PrintfTest ====" << endl << endl;
    unsigned char a = 23;
    unsigned char b = 24;
    printf("%d %d\n", a, b);
    printf("%.4f", 3.0);
}

void MemoryLeakTest() {
    cout << "==== MemoryLeakTest ====" << endl << endl;
    // 이것을 하면 프로그램 종료시 디버그 출력창에 메모리 릭 정보를 아래와 같이 표시해 준다.
    // Detected memory leaks!
    // Dumping objects ->
    // {165} normal block at 0x0000018484721D80, 40 bytes long.
    //  Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 할당 번호 {165} 를 아래에 함수에 넣어주면 해당 할당시 런타임 에러를 발생시켜 준다. 
    //_CrtSetBreakAlloc(161);

    auto a = new int[10];
    auto b = new int[10];
    auto c = new int[20];

    delete[] a;
    delete[] c;

    _CrtDumpMemoryLeaks();    // _CrtSetDbgFlag 대시 프로그램 끝나기 전에 이걸 호출 해도 됨
}

void UnrefFunctionList() {
    // Project 속성에서
    // C/C++ -> 코드생성 -> 함수수준링크사용 : 예(/Gy)
    // 링커 -> 최적화 -> 참조 : Yes(/OPT:REF)
    // 링커 -> 명령줄 -> 추가옵션 : /VERBOSE:REF 추가
}

void Func(int a) { cout << "정수를 인자로 받는 함수" << endl; }
void Func(int* a) { cout << "포인터를 인자로 받는 함수" << endl; }
void NullPtrTest() {
    cout << "==== NullPtrTest ====" << endl << endl;
    Func(1);
    Func(NULL); // 이것을 정수 인자 함수가 호출된다. 포인터 인자 함수를 널포인터 인자로 호출 하고 싶을때 난감
    Func(nullptr);
}

class Car {
private:
    wstring name;
public:
    float speed;
    float fuel;
    Car(wstring name = L"NoName") {
        this->name = name;
        wcout << L"Make Car[" << this->name << L"]" << endl;
    };
    ~Car() {
        wcout << L"Destroy Car[" << this->name << L"]" << endl;
    };
};

void SharedPtrTest() {
    cout << "==== SharedPtrTest ====" << endl << endl;
    wcout.imbue(locale("korean"));
    auto car = Car(L"아반떼");
    auto car2 = Car();
    auto sp = shared_ptr<Car>(new Car(L"소나타"));
    auto sp2 = shared_ptr<Car>(new Car(L"마티즈"));
    auto sp3(sp);
    auto sp4 = sp2;
}

void SharedPtrInitTest() {
    cout << "==== SharedPtrInitTest ====" << endl << endl;
    wcout.imbue(locale("korean"));
    shared_ptr<Car> sp1(new Car(L"그랜저"));
    auto sp2 = shared_ptr<Car>(new Car(L"소나타"));
    auto sp3 = make_shared<Car>(L"아반떼");
}

shared_ptr<Car> fullRentCar;
void SharedPtrVectorTest() {
    cout << "==== SharedPtrVectorTest ====" << endl << endl;
    wcout.imbue(locale("korean"));
    vector<shared_ptr<Car>> carList;
    carList.push_back(shared_ptr<Car>(new Car(L"그랜저")));
    carList.push_back(shared_ptr<Car>(new Car(L"소나타")));
    carList.push_back(shared_ptr<Car>(new Car(L"아반떼")));
    carList.push_back(shared_ptr<Car>(new Car(L"마티즈")));
    auto rentCar1 = carList[1];
    fullRentCar = carList[2];
}

class Graphic {
public:
    Graphic() { cout << "그래픽 생성" << endl; }
    virtual ~Graphic() { cout << "그래픽 파괴" << endl; }
    virtual void Draw() { cout << "그래픽 오브젝트입니다." << endl; }
};

class Line : public Graphic {
public:
    Line() { cout << "선 생성" << endl; }
    ~Line() { cout << "선 파괴" << endl; }
    void Draw() { cout << "선을 긋습니다." << endl; }
};

class Circle : public Graphic {
public:
    Circle() { cout << "원 생성" << endl; }
    ~Circle() { cout << "원 파괴" << endl; }
    void Draw() { cout << "동그라미 그렸다 치고." << endl; }
};

class Rect : public Graphic {
public:
    Rect() { cout << "사각형 생성" << endl; }
    ~Rect() { cout << "사각형 파괴" << endl; }
    void Draw() { cout << "요건 사각형입니다." << endl; }
};

void del(Graphic* g) {
    delete g;
}

void VectorTest() {
    cout << "==== VectorTest ====" << endl << endl;
    vector<int> intlist;
    intlist.push_back(1);
    intlist.push_back(2);
    intlist.push_back(3);
    intlist.push_back(4);
    //for (int i : intlist)
    //   cout << i << endl;
    for_each(intlist.begin(), intlist.end(), [](int a) { cout << a << endl; });

    vector<Graphic*> vg;
    vg.push_back(new Graphic());
    vg.push_back(new Rect());
    vg.push_back(new Circle());
    vg.push_back(new Line());

    vector<Graphic*>::iterator it;
    for (it = vg.begin(); it != vg.end(); it++) {
        (*it)->Draw();
    }
    for_each(vg.begin(), vg.end(), del);
}

void VectorTest2() {
    cout << "==== VectorTest2 ====" << endl << endl;
    vector<int> intlist;
    intlist.push_back(1);
    intlist.push_back(2);
    intlist.push_back(3);
    intlist.push_back(4);
    //for (int i : intlist)
    //   cout << i << endl;
    for_each(intlist.begin(), intlist.end(), [](int a) { cout << a << endl; });

    vector<Graphic> vg;
    vg.push_back(Graphic());
    vg.push_back(Rect());
    vg.push_back(Circle());
    vg.push_back(Line());

    vector<Graphic>::iterator it;
    for (it = vg.begin(); it != vg.end(); it++) {
        (*it).Draw();
    }
    //for_each(vg.begin(), vg.end(), del);
}

void VectorTest3() {
    cout << "==== VectorTest3 ====" << endl << endl;
    vector<int> intlist;
    intlist.push_back(0);
    intlist.push_back(1);
    intlist.push_back(2);
    intlist.push_back(3);
    intlist.push_back(4);
    for (size_t i = 0; i < intlist.size(); i++)
        cout << intlist[i] << endl;
}

void VectorTest4() {
    cout << "==== VectorTest4 ====" << endl << endl;
    vector<int> ints;
    size_t cap = ints.capacity();
    cout << "size : " << ints.size() << ", cap : " << cap << endl;
    for (int i = 0; i < 100000; i++) {
        ints.push_back(i);
        size_t cap2 = ints.capacity();
        //if (cap2 != cap) {
        cap = cap2;
        cout << "size : " << ints.size() << ", cap : " << cap << ", &ints.front() : " << &ints.front() << endl;
        //}
    }
}

void VectorTestString() {
    cout << "==== VectorTestString ====" << endl << endl;
    vector<string> strList;
    strList.push_back("aaa");
    strList.push_back("bbb");
    strList.push_back("ccc");
    string str1 = "str1";
    strList.push_back(str1);
    string str2("_4_str2");
    strList.push_back(str2);
    strList.push_back(string("str3"));
    for (auto it = strList.begin(); it != strList.end(); it++) {
        cout << *it << endl;
    }
}

void ChronoTestDuration() {
    cout << "==== ChronoTestDuration ====" << endl << endl;
    auto t0 = chrono::high_resolution_clock::now();
    this_thread::sleep_for(chrono::seconds(3));
    auto t1 = chrono::high_resolution_clock::now();
    auto diff = t1 - t0;
    cout << "diff: " << diff.count() << endl;
    cout << "sec : " << chrono::duration_cast<chrono::seconds>(diff).count() << endl;
    cout << "ms  : " << chrono::duration_cast<chrono::milliseconds>(diff).count() << endl;
    cout << "us  : " << chrono::duration_cast<chrono::microseconds>(diff).count() << endl;
    cout << "ns  : " << chrono::duration_cast<chrono::nanoseconds>(diff).count() << endl;
}

void ChronoTestNow() {
    cout << "==== ChronoTestNow ====" << endl << endl;
    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    char str[26];
    ctime_s(str, sizeof str, &t);
    cout << str << endl;
}

struct MyCar {
    int speed;
    int fuel;
    int direction;
};

MyCar GetMyCar() {
    MyCar car;
    cout << "object from callee : " << &car << endl;
    return car;
}

void ObjectReturnTest() {
    cout << "==== ObjectReturnTest ====" << endl << endl;
    MyCar car = GetMyCar();
    cout << "object from caller : " << &car << endl;
}

int fun() {
    for (int i = 1; i <= 10; i++) {
        cout << "fun[" << i << "]" << endl;
    }

    return 200;
}

void PromiseFutureTest() {
    cout << "==== PromiseFutureTest ====" << endl << endl;
    //future<int> fut = async(fun);
    future<int> fut = async(launch::async, fun);  // 비동기로 실행
    //future<int> fut = async(launch::deferred, fun);

    for (int i = 1; i <= 10; i++) {
        cout << "main[" << i << "]" << endl;
    }
    cout << endl;

    int result = fut.get();   // 비동기 작업이 완료될때까지 대기하고 리턴값 받음

    cout << "result : " << result << endl;
}

void ConstPointerTest() {
    cout << "==== ConstPointerTest ====" << endl << endl;
    int a = 5;
    int b = 10;
    const int* ptr1 = &a;    // int 변경 불가
    //*ptr1 = 10;             // 포인터가 가리키는 값 변경 불가
    ptr1 = &b;              // 포인터 변수 변경 가능

    int* const ptr2 = &a;   // 포인터 변경 불가
    *ptr2 = 10;             // 포인커가 가리키는 값은 변경 가능
    //ptr2 = &b;              // 포인터 변수 변경 가능
}

class GraphicSet {
public:
    Line line;
    Circle circle;
    Rect rect;
    Rect rect2;
    GraphicSet(string log) {
        cout << log << endl;
    }
    static GraphicSet staticObj;
};

//GraphicSet GraphicSet::staticObj("Static Object");
//GraphicSet globalObj("Global Object");

void GraphicSetTest() {
    cout << "==== GraphicSetTest ====" << endl << endl;
    GraphicSet localObj("Local Object");
    int a = 1;
}

class MyClass {
public:
    static int num;
};
int MyClass::num;
void StaticMemberTest() {
    cout << "==== StaticMemberTest ====" << endl << endl;
    MyClass::num = 100;
    cout << MyClass::num << endl;
}

class CarBase {
public:
    virtual void Run();
};

class Truck : CarBase {
public:
    virtual void Run();
};

class Bus : CarBase {
public:
    virtual void Run();
};

class Taxi : CarBase {
public:
    virtual void Run();
};

class SomeSet {
public:
    virtual void SomeDo() {

    }
};

class CarSet : SomeSet {
public:
    CarBase car;
    Bus bus;
    Truck truck;
    Taxi taxi;
    virtual void SomeDo() {

    }
};

extern CarSet carSet;

CarSet carSet;
void CarBase::Run() {}
void Bus::Run() {}
void Taxi::Run() {}
void Truck::Run() {}

void CarSetTest() {
    cout << "==== CarSetTest ====" << endl << endl;
    carSet.bus.Run();
}

wstring StrToWstr(string const& str) {
    size_t len = str.size();
    size_t bufSize = len + 1;
    wchar_t* wc = new wchar_t[bufSize];
    size_t lenDone;
    mbstowcs_s(&lenDone, wc, bufSize, str.c_str(), len);
    wstring wstr(wc);
    delete[] wc;
    return wstr;
}

string WstrToStr(wstring const& wstr) {
    size_t len = wstr.size();
    size_t bufSize = len * 2 + 1;
    char* c = new char[bufSize];
    size_t lenDone;
    wcstombs_s(&lenDone, c, bufSize, wstr.c_str(), len);
    string str(c);
    delete[] c;
    return str;
}

wstring mbs_to_wcs(string const& str, locale const& loc = locale("")) {
    typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;
    codecvt_t const& codecvt = use_facet<codecvt_t>(loc);
    mbstate_t state;
    vector<wchar_t> buf(str.size() + 1);
    char const* in_next = str.c_str();
    wchar_t* out_next = &buf[0];
    codecvt_t::result r = codecvt.in(state, str.c_str(), str.c_str() + str.size(), in_next, &buf[0], &buf[0] + buf.size(), out_next);
    return wstring(&buf[0]);
}

string wcs_to_mbs(wstring const& str, locale const& loc = locale("")) {
    typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;
    codecvt_t const& codecvt = use_facet<codecvt_t>(loc);
    mbstate_t state;
    vector<char> buf((str.size() + 1) * codecvt.max_length());
    wchar_t const* in_next = str.c_str();
    char* out_next = &buf[0];
    codecvt_t::result r = codecvt.out(state, str.c_str(), str.c_str() + str.size(), in_next, &buf[0], &buf[0] + buf.size(), out_next);
    return string(&buf[0]);
}


void StringConvertTest() {
    cout << "==== StringConvertTest ====" << endl << endl;
    //locale::global(locale("ko-KR"));
    //setlocale(LC_ALL, "ko-KR");

    const char* _0_ch = "안녕AB";
    const wchar_t* _1_wch = L"안녕AB";

    string  _2_str = "안녕AB";
    wstring _3_wstr = L"안녕AB";

    string _4_str2 = WstrToStr(_3_wstr);
    wstring _5_wstr2 = StrToWstr(_2_str);

    string _6_str2 = wcs_to_mbs(_3_wstr);
    wstring _7_wstr2 = mbs_to_wcs(_2_str);

    //string _8_str3 = string(_3_wstr.begin(), _3_wstr.end());
    //wstring _9_wstr3 = wstring(_2_str.begin(), _2_str.end());

    cout << "char* : " << _0_ch << endl;
    wcout << "wchar_t* : " << _1_wch << endl;

    cout << "string : " << _2_str << endl;
    wcout << L"wstring : " << _3_wstr << endl;

    cout << "WstrToStr : " << _4_str2 << endl;
    wcout << L"StrToWstr : " << _5_wstr2 << endl;

    cout << "wcs_to_mbs : " << _6_str2 << endl;
    wcout << L"mbs_to_wcs : " << _7_wstr2 << endl;

    //cout << "string(iter, iter) : " << _8_str3 << endl;
    //wcout << L"wstring(iter, iter) : " << _9_wstr3 << endl;
}

void RangeBasedForTest() {
    cout << "==== RangeBasedForTest ====" << endl << endl;
    int arr1[] = { 1,2,3,4,5 };
    // int *arr1 = new int[5];  // error
    for (auto a : arr1) {
        cout << a << " ";
    }
    cout << endl;

    for (auto& a : arr1) {
        a = a * 2;
    }

    for (auto a : arr1) {
        cout << a << " ";
    }
    cout << endl;
}

template<typename T>
void PrintVector(vector<T> arr) {
    for_each(arr.begin(), arr.end(), [](auto num) { cout << num << " "; });
}

void MapTest() {
    cout << "==== MapTest ====" << endl << endl;
    vector<int> inarr = { 1,2,3,4,5 };
    PrintVector(inarr);

    cout << endl;

    vector<int> outarr;
    outarr.resize(5);
    transform(inarr.begin(), inarr.end(), outarr.begin(), [](auto num) { return num * 2; });
    PrintVector(outarr);

    cout << endl;

    copy_if(inarr.begin(), inarr.end(), outarr.begin(), [](auto num) { return num % 2 == 1; });
    PrintVector(outarr);
}

void ForTest() {
    cout << "==== ForTest ====" << endl << endl;
    vector<wstring> msg{ L"Hello", L"C++", L"World", L"from", L"VS Code!" };

    for (const auto& word : msg) {
        wcout << word << " ";
    }
    cout << endl;
}

void RemainderTest() {
    cout << "==== RemainderTest ====" << endl << endl;
    for (int i = -10; i <= 10; i++) {
        cout << i << " / 2 = " << i / 2 << ", " << i << " %% 2 = " << i % 2 << endl;
    }
}

class Student {
public:
    string name;
    int age;
    Student(string name, int age) {
        this->name = name;
        this->age = age;
    }
    friend ostream& operator <<(ostream& c, const Student& T);
};

ostream& operator <<(ostream& c, const Student& T) {
    return c << T.name << ":" << T.age;
}

void VectorSortTest() {
    cout << "==== VectorSortTest ====" << endl << endl;
    cout << "vector<int> sort" << endl;
    vector<int> intList{ 2, 9, 1, 5, 4, 7, 6, 3, 0, 8, };
    sort(intList.begin(), intList.end(), [](int a, int b) -> bool { return a < b; });
    for (int item : intList)
        cout << item << " ";
    cout << endl << endl;

    cout << "vector<string> sort" << endl;
    vector<string> stringList = { "s4", "s7", "s6", "s8", "s2", "s0", "s9", "s5", "s3", "s1", };
    sort(stringList.begin(), stringList.end(), [](string a, string b) -> bool { return a < b; });
    for (string item : stringList)
        cout << item << " ";
    cout << endl << endl;

    vector<Student> studentList = { { "s4", 2 }, { "s7", 9 }, { "s6", 1 }, { "s8", 5 }, { "s2", 4 }, { "s0", 7 }, { "s9", 6 }, { "s5", 3 }, { "s3", 0 }, { "s1", 8 }, };

    cout << "vector<Student> sort by age" << endl;
    sort(studentList.begin(), studentList.end(), [](Student a, Student b) -> bool { return a.age < b.age; });
    for (Student item : studentList)
        cout << item << " ";
    cout << endl << endl;

    cout << "vector<Student> sort by name" << endl;
    sort(studentList.begin(), studentList.end(), [](Student a, Student b) -> bool { return a.name < b.name; });
    for (Student item : studentList)
        cout << item << " ";
    cout << endl << endl;
}

ostream& operator <<(ostream& c, const MyCar& T) {
    return c << "{" << T.speed << "," << T.fuel << "," << T.direction << "}";
}

void ReferenceTest() {
    cout << "==== ReferenceTest ====" << endl << endl;
    MyCar car1 = { 1,2,3, };
    MyCar car2 = { 11,12,13, };
    MyCar& refCar = car1;
    cout << car1 << " " << car2 << " " << refCar << endl;
    refCar = car2;
    cout << car1 << " " << car2 << " " << refCar << endl;
}

void StructInitialize() {
    cout << "==== StructInitialize ====" << endl << endl;
    MyCar car1;
    car1.speed = 1;
    car1.fuel = 2;
    car1.direction = 3;

    MyCar car2 = { 1,2,3, };
    MyCar car3{ 1,2,3 };
}

template<typename IT>
void PrintIterator(IT s, IT e) {
    return;
    for (auto it = s; it != e; it++) {
        cout << *it << " ";
    }
    cout << endl;
}

template<typename T>
void PrintIndex(T seq, size_t s, size_t e) {
    for (size_t i = s; i != e; i++) {
        cout << seq[i] << " ";
    }
    cout << endl;
}

void IteratorTest() {
    cout << "==== IteratorTest ====" << endl << endl;

    int ari[] = { 1,2,3,4,5 };
    vector<int> vi(&ari[0], &ari[5]);
    list<int> li(&ari[0], &ari[5]);

    cout << "iterate array" << endl;
    PrintIterator(&ari[0], &ari[5]);
    cout << "iterate vector" << endl;
    PrintIterator(vi.begin(), vi.end());
    cout << "iterate list" << endl;
    PrintIterator(li.begin(), li.end());

    cout << "index array" << endl;
    PrintIndex(ari, 0, 5);
    cout << "index vector" << endl;
    PrintIndex(vi, 0, vi.size());
    // error : list는 인덱싱 연산자([])가 정의되어 있지 않다.
    //cout << "index list" << endl;
    //PrintIndex(li, 0, li.size());
}

void StlFindTest() {
    cout << "==== StlFindTest ====" << endl << endl;

    int arr[] = { 1,2,3,4,5 };
    vector<int> vi = { 1,2,3,4,5 };
    list<int> li = { 1,2,3,4,5 };

    cout << (find(vi.begin(), vi.end(), 4) == vi.end() ? "없다." : "있다.") << endl;
    cout << (find(li.begin(), li.end(), 8) == li.end() ? "없다." : "있다.") << endl;
    cout << (find(&arr[0], &arr[5], 3) == &arr[5] ? "없다." : "있다.") << endl;
}

void StlSortTest() {
    cout << "==== StlSortTest ====" << endl << endl;

    vector<int> vi = { 2,8,5,1,9 };

    sort(vi.begin(), vi.end());
    for (int i : vi)
        cout << i << endl;
}

void StlReverseTest() {
    cout << "==== StlReverseTest ====" << endl << endl;

    vector<int> vi = { 1,2,3,4,5,6,7,8,9 };

    for (int i : vi)
        cout << i << " ";
    cout << endl;

    reverse(vi.begin(), vi.end());
    for (int i : vi)
        cout << i << " ";
}

void StlRandomShuffleTest() {
    cout << "==== StlRandomShuffleTest ====" << endl << endl;

    vector<int> vi = { 1,2,3,4,5,6,7,8,9 };

    for (int i : vi)
        cout << i << " ";
    cout << endl;

    random_shuffle(vi.begin(), vi.end());
    for (int i : vi)
        cout << i << " ";
}

class MyMesmerize {
public:
    MyMesmerize(int a, int b) {
        cout << "MyMesmerize is consturcted." << endl;
    }
    ~MyMesmerize() {
        cout << "MyMesmerize is destroyed." << endl;
    }
};

void SomeMesmerizeTest1() {
    cout << "==== SomeMesmerizeTest1 ====" << endl << endl;
    //MyMesmerize obj();  // 햇갈리지 말자. MyMesmerize객체 생성이 아니고 MyMesmerize를 리턴하는 파라미터 없는 obj함수 선언. 아무것도 안함.
    // obj(); // 이것은 에러. 위에서 함수선언은 했지만 정의를 안했으므로 링크 에러 발생
    MyMesmerize obj1(1, 2);
    MyMesmerize obj2 = { 1,2 };
    MyMesmerize obj3{ 2,3 };
}

void VectorElementAccessTest() {
    cout << "==== VectorElementAccessTest ====" << endl << endl;
    vector<int> intList = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    for (int i : intList)
        cout << i << " ";
    cout << endl;

    // reference at (size_type n); 함수로 접근
    int a = intList.at(3);  // get
    cout << a << endl;
    intList.at(3) = 7;      // set
    for (int i : intList)
        cout << i << " ";
    cout << endl;

    // reference operator[] (size_type n); 연산자로 접근
    int b = intList[4];     // get
    cout << b << endl;
    intList[4] = 8;         // set
    for (int i : intList)
        cout << i << " ";
    cout << endl;

    // iterator 통해서 접근
    auto it = intList.begin();
    advance(it, 5);
    int c = *it;            // get
    cout << c << endl;
    *it = 9;                // set
    for (int i : intList)
        cout << i << " ";
    cout << endl;
}

void ListElementAccessTest() {
    cout << "==== ListElementAccessTest ====" << endl << endl;
    list<int> intList = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    for (int i : intList)
        cout << i << " ";
    cout << endl;

    // reference at (size_type n); 함수로 접근 불가
    // reference operator[] (size_type n); 연산자로 접근 불가

    // iterator 통해서 접근
    auto it = intList.begin();
    advance(it, 5);
    int c = *it;            // get
    cout << c << endl;
    *it = 9;                // set
    for (int i : intList)
        cout << i << " ";
    cout << endl;
}

void fPrint(int a) {
    cout << a << " ";
}

struct SPrint {
    void operator()(int a) const {
        cout << a << " ";
    }
};

struct SSum {
    int sum;
    SSum() {
        sum = 0;
    }
    void operator()(int a) {
        sum += a;
    }
};

struct SPrintString {
    string mes;
    SPrintString(string m) : mes(m) {}
    void operator()(int a) const {
        cout << mes << a << endl;
    }
};

template <typename T>
struct SomeClass {
    T functor;
    SomeClass(T _functor) {
        functor = _functor;
    };
    void Call(int a) {
        functor(a);
    }
};

void ForeachFunctorTest() {
    cout << "==== ForeachFunctorTest ====" << endl << endl;
    vector<int> intList = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    for_each(intList.begin(), intList.end(), fPrint);
    cout << endl;
    for_each(intList.begin(), intList.end(), SPrint());
    cout << endl;
    for_each(intList.begin(), intList.end(), [](int a) -> void { cout << a << " "; });
    cout << endl;

    SSum sumObj = for_each(intList.begin(), intList.end(), SSum());  // 함수 객체의 복사본이 넘어가기 때문에 그 복사본을 리턴받아 와야 한다.
    cout << "sum = " << sumObj.sum << endl;

    for_each(intList.begin(), intList.end(), SPrintString(string("요소값은 ")));
    for_each(intList.begin(), intList.end(), SPrintString(string("다른 메시지 ")));
    // SomeClass<SPrint> s1(SPrint());           // 이거 애매 모호 함, SPrint()를 SPrint(*)() 함수 포인터 타입으로 인식하여 객체 생성구문이 아닌 함수 선언 구문으로 이식함
    SomeClass<SPrint> s1 = SomeClass<SPrint>(SPrint());           // 템플릿 변수 선언
    SomeClass<void(*)(int)> s2(fPrint);    // 템플릿 변수 선언
    s1.Call(3);
    s2.Call(4);
}

struct IsKim {
    bool operator()(string name) const {
        return (strncmp(name.c_str(), "김", 2) == 0);
    }
};
void FindIfTest() {
    cout << "==== ForeachFunctorTest ====" << endl << endl;
    vector<string> nameList = { "김유신", "이순신", "성삼문", "장보고", "조광조", "신숙주", "김홍도", "정도전", "이성계", "정몽주" };

    vector<string>::iterator it = find_if(nameList.begin(), nameList.end(), IsKim());
    if (it == nameList.end()) {
        cout << "김가 없다." << endl;
    }
    else {
        cout << *it << "이(가) 있다." << endl;
    }

    vector<string>::iterator it2 = find_if(nameList.begin(), nameList.end(), [](string name) ->bool { return (strncmp(name.c_str(), "신", 2) == 0); });
    if (it2 == nameList.end()) {
        cout << "신가 없다." << endl;
    }
    else {
        cout << *it2 << "이(가) 있다." << endl;
    }

    vector<string>::iterator it3;
    for (it3 = nameList.begin();; it3++) {
        it3 = find_if(it3, nameList.end(), IsKim());
        if (it3 == nameList.end()) break;
        cout << *it3 << "이(가) 있다" << endl;
    }
}

void PredefFunctorTest() {
    cout << "==== PredefFunctorTest ====" << endl << endl;
    int r1 = plus<int>()(3, 4);
    cout << r1 << endl;

    vector<string> vs = { "STL", "MFC", "owl", "html", "pascal", "Ada", "Delphi", "C/C++", "Python", "basic" };
    sort(vs.begin(), vs.end(), less<string>());
    for (auto v : vs)
        cout << v << " ";
}

struct StringCompareNoCase {
    bool operator()(string& a, string& b) {
        return _stricmp(a.c_str(), b.c_str()) < 0;
    }
};
void StringSortNoCaseTest() {
    cout << "==== StringSortNoCaseTest ====" << endl << endl;
    vector<string> vs = { "STL", "MFC", "owl", "html", "pascal", "Ada", "Delphi", "C/C++", "Python", "basic" };
    sort(vs.begin(), vs.end(), StringCompareNoCase());
    for (auto v : vs)
        cout << v << " ";
}

bool IntCompare(int a, int b) {
    return a < b;
}

struct SIntCompare {
    bool operator()(int a, int b) {
        return a < b;
    }
};

int cmpfunc(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void VectorSortSpeedTest() {
    cout << "==== VectorSortSpeedTest ====" << endl << endl;
    cout << "input : int[10000000]" << endl << endl;

    int num = 10000000;
    int* arr1 = new int[num];

    cout << "generate" << endl;
    iota(arr1, arr1 + num, 0);
    PrintIterator(arr1, arr1 + num);

    cout << endl;
    cout << "shuffle" << endl;
    random_shuffle(arr1, arr1 + num);
    PrintIterator(arr1, arr1 + num);

    int* arr2 = new int[num];
    copy(arr1, arr1 + num, arr2);
    PrintIterator(arr2, arr2 + num);

    int* arr3 = new int[num];
    copy(arr1, arr1 + num, arr3);
    PrintIterator(arr3, arr3 + num);

    int* arr4 = new int[num];
    copy(arr1, arr1 + num, arr4);
    PrintIterator(arr4, arr4 + num);

    int* arr5 = new int[num];
    copy(arr1, arr1 + num, arr5);
    PrintIterator(arr5, arr5 + num);

    int* arr6 = new int[num];
    copy(arr1, arr1 + num, arr6);
    PrintIterator(arr6, arr6 + num);

    chrono::time_point<chrono::system_clock> st;
    chrono::duration<double> sec;

    cout << endl;
    cout << "sort" << endl;
    st = chrono::system_clock::now();
    sort(arr1, arr1 + num, IntCompare);
    sec = chrono::system_clock::now() - st;
    cout << "stl::sort function : " << sec.count() << "sec" << endl;
    PrintIterator(arr1, arr1 + num);

    st = chrono::system_clock::now();
    sort(arr2, arr2 + num, SIntCompare());
    sec = chrono::system_clock::now() - st;
    cout << "stl::sort functor  : " << sec.count() << "sec" << endl;
    PrintIterator(arr2, arr2 + num);

    st = chrono::system_clock::now();
    sort(arr3, arr3 + num, [](int a, int b) -> bool { return a < b; });
    sec = chrono::system_clock::now() - st;
    cout << "stl::sort lambda   : " << sec.count() << "sec" << endl;
    PrintIterator(arr3, arr3 + num);

    auto lambdaObj = [](int a, int b) -> bool { return a < b; };
    st = chrono::system_clock::now();
    sort(arr4, arr4 + num, lambdaObj);
    sec = chrono::system_clock::now() - st;
    cout << "stl::sort lambda object  : " << sec.count() << "sec" << endl;
    PrintIterator(arr4, arr4 + num);

    st = chrono::system_clock::now();
    qsort(arr5, num, sizeof(int), cmpfunc);
    sec = chrono::system_clock::now() - st;
    cout << "qsort     function : " << sec.count() << "sec" << endl;
    PrintIterator(arr5, arr5 + num);

    st = chrono::system_clock::now();
    qsort(arr6, num, sizeof(int), [](const void* pt1, const void* pt2)->int { return *(int*)pt1 - *(int*)pt2; });
    sec = chrono::system_clock::now() - st;
    cout << "qsort     lambda   : " << sec.count() << "sec" << endl;
    PrintIterator(arr6, arr6 + num);

    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;
    delete[] arr5;
    delete[] arr6;
}

void RoundTest() {
    cout << "==== RoundTest ====" << endl << endl;

    cout << "input\n";
    cout << "      round\n";
    cout << "      :     floor\n";
    cout << "      :     :     (int)\n";
    cout << "      :     :     :     floor(+0.5)\n";
    cout << "      :     :     :     :     (int)(+0.5)\n";
    double start = -10;
    double end = 10;
    for (double item = start; item < end; item += 0.5) {
        auto Round = round(item);
        auto Floor = floor(item);
        auto CastInt = (int)item;
        auto AddHalfFloor = floor(item + 0.5);
        auto AddHalfIntCast = (int)(item + 0.5);
        cout.width(5); cout << item << " : ";
        cout.width(3); cout << Round << " : ";
        cout.width(3); cout << Floor << " : ";
        cout.width(3); cout << CastInt << " : ";
        cout.width(3); cout << AddHalfFloor << " : ";
        cout.width(3); cout << AddHalfIntCast << endl;
    }
}

void NegativeModulusTest() {
    cout << "==== NegativeModulusTest ====" << endl << endl;
    int n = 3;
    for (int i = -20; i < 20; i++) {
        int mod1 = i % n;
        int mod2 = (i % n + n) % n;
        cout << i << " : " << mod1 << " " << mod2 << endl;
    }
}

void SizeOfPrimitiveTest() {
    cout << "==== SizeOfPrimitiveTest ====" << endl << endl;
    cout << "sizeof(bool) : " << sizeof(bool) << endl;
    cout << "sizeof(long) : " << sizeof(long) << endl;
    cout << "sizeof(long double) : " << sizeof(long double) << endl;
    cout << "sizeof(long long) : " << sizeof(long long) << endl;
    function<void()> f;
}

void FillTest() {
    cout << "==== FillTest ====" << endl << endl;
    auto vs = vector<int>(10);
    fill(vs.begin(), vs.end(), 3);
    PrintIterator(vs.begin(), vs.end());
}

void GenerateTest() {
    cout << "==== GenerateTest ====" << endl << endl;
    auto vs = vector<int>(10);
    int i = 0;
    generate(vs.begin(), vs.end(), [&i] { return i++; });
    PrintIterator(vs.begin(), vs.end());
}

void IotaTest() {
    cout << "==== IotaTest ====" << endl << endl;
    auto vs = vector<int>(10);
    iota(vs.begin(), vs.end(), 0);
    PrintIterator(vs.begin(), vs.end());
}

void TransformTest() {
    cout << "==== TransformTest ====" << endl << endl;
    auto vs0 = vector<int>(10);
    int i = 0;
    generate(vs0.begin(), vs0.end(), [&i] { return i++; });
    PrintIterator(vs0.begin(), vs0.end());
    cout << endl;

    auto vs1 = vector<string>(vs0.size());
    transform(vs0.begin(), vs0.end(), vs1.begin(), [](int i) { return (i % 2 == 0) ? "even" : "odd"; });
    PrintIterator(vs1.begin(), vs1.end());
}

class B {
protected:
    string name;
public:
    B(string _name) : name(_name) {
        cout << "Base construct : " << name << endl;
    }
    virtual ~B() {
        cout << "Base destruct : " << name << endl;
    }
};

class D : public B {
public:
    D(string _name) : B(_name) {
        cout << "Derived construct : " << name << endl;
    }
    ~D() {
        cout << "Derived destruct : " << name << endl;
    }
};

void InheritConstructorTest() {
    cout << "==== InheritConstructorTest ====" << endl << endl;
    B* b = new D("starless");
    delete b;
}

void TryCatchTest() {
    cout << "==== TryCatchTest ====" << endl << endl;
    try {
        cout << "try start" << endl;

        // 나누기 0 못잡는다
        //int a = 3;
        //int b = 0;
        //cout << a / b << endl;

        // 배열 인덱스 못잡는다
        //int arr[] = { 0,1,2,3 };
        //cout << arr[-1] << endl;

        // 벡터 인덱스 못잡는다
        //vector<int> vec = { 0,1,2,3 };
        //cout << vec[-1] << endl;

        // stoi는 잡는다
        int n = stoi("abc");
        cout << n << endl;

        cout << "try end" << endl;
    }
    catch (const exception & e) {
        cout << "catch exception : " << e.what() << endl;
    }
}

void LambdaTest1() {
    cout << "==== LambdaTest1 ====" << endl << endl;
    vector<float> flist{ 5, 7, 4, 7, 3, 7, 6, 3, 6, 8, 3, 1, 0, };
    //sort(flist.begin(), flist.end(), [](float a, float b) ->bool { return a < b; });
    sort(flist.begin(), flist.end(), [](float a, float b) { return a < b; });
    PrintIterator(flist.begin(), flist.end());
}

void LambdaTest2() {
    cout << "==== LambdaTest2 ====" << endl << endl;
    int x = 1;
    int y = 2;
    auto func = [=]() mutable throw() ->int {
        int n = x + y;
        x = y;
        y = n;
        return n;
    };
    int ret = func();
    cout << ret << endl;
    cout << x << endl;
    cout << y << endl;
}

void StringAppendTest() {
    cout << "==== StringAppendTest ====" << endl << endl;
    string str;
    ofstream ofs("c:\\test\\StringAppendTest2.txt");
    ofs << " c_str | data | length | size | capacity" << endl;
    for (int i = 0; i < 10000; i++) {
        str += "a";
        ofs << (void*)str.c_str() << " " << (void*)str.data() << " " << str.length() << " " << str.size() << " " << str.capacity() << endl;
    }
    ofs.close();
    cout << "writing to c:\\test\\StringAppendTest2.txt finished" << endl;
}

class YourClass {
private:
    string name;
public:
    YourClass(string _name) : name(_name) {
        cout << "일반 생성자" << endl;
        PrintName();
    }

    YourClass(const YourClass& a) : name(a.name) {
        cout << "복사 생성자" << endl;
        PrintName();
    }

    YourClass(YourClass&& a) : name(a.name) {
        cout << "이동 생성자" << endl;
        PrintName();
    }

    ~YourClass() {
        cout << "소멸자" << endl;
        PrintName();
    }

    YourClass& operator = (const YourClass& a) {
        name = a.name;
        cout << "대입 연산자" << endl;
        PrintName();
        return *this;
    }

    YourClass& operator = (YourClass&& a) {
        name = a.name;
        cout << "이동 연산자" << endl;
        PrintName();
        return *this;
    }

    void PrintName() {
        cout << "이름 출력 : " << name << endl;
    }
};

YourClass MoveMethod(YourClass aobj) {
    YourClass robj("이순신");
    return robj;
}

void MoveTest() {
    YourClass obj1("홍길동");
    YourClass obj2 = MoveMethod(obj1);
}

void MoveConstructorOperator() {
    auto obj1 = YourClass("꼬북좌");    // 생성자
    auto obj2 = obj1;           // 복사 생성자
    obj2 = obj1;                // 대입 연산자
    obj1.PrintName();
    auto obj3 = move(obj1);    // 이동 생성자
    obj1.PrintName();
    obj3 = move(obj1); // 이동 연산자
}

void RefTest() {
    int a = 1;
    int& refA = a;
    a = 3;
    int b = 2;
    refA = b;
    b = 4;
    int i = 4, j = 5;
    ((i < 3) ? i : j) = 8;
    string s = string("h") + "e" + "ll" + "o";
    cout << s << endl;
    // 정리:
    // lvalue는 대입연산에서 왼쪽에 올 수 있는 값, 주소가 있는 값, 변수, *연산 결과
    // rvalue는 대입연산에서 왼쪽에 울 수 없는 값, 주소가 없는 값, 상수, 산술 연산 결과, &연산 결과
}

void DebugViewTest() {
    auto vs = vector<int>(10);
    iota(vs.begin(), vs.end(), 0);
    for (auto v : vs) {
        wstring str = to_wstring(v);
        OutputDebugString(str.c_str()); // DebugView.exe를 실행시켜놓고 있으면 거기에 이게 출력 된다
    }
}

/* swap: interchange v[i] and v[j] */
void swap(int v[], int i, int j) {
    int temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* qsort: sort v[left]...v[right] into increasing order */
void QuickSort_tcpl(int v[], int left, int right) {
    int i, last;
    void swap(int v[], int i, int j);
    if (left >= right) /* do nothing if array contains */
        return; /* fewer than two elements */
    swap(v, left, (left + right) / 2); /* move partition elem */
    last = left; /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (v[i] < v[left])
            swap(v, ++last, i);
    swap(v, left, last); /* restore partition elem */
    QuickSort_tcpl(v, left, last - 1);
    QuickSort_tcpl(v, last + 1, right);
}

void QuickSortTest() {
    int arr[] = { 14, 4, 19, 13, 17, 3, 12, 6, 10, 1, 5, 2, 9, 16, 18, 15, 8, 0, 11, 7 };
    for (int i = 0; i < 20; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");
    QuickSort_tcpl(arr, 0, 19);
    for (int i = 0; i < 20; i++) {
        printf("%d ", arr[i]);
    }
}

void ForEachTest() {
    vector<int> data(10);
    iota(begin(data), end(data), 1);

    for_each(begin(data), end(data), [](auto value) { cout << value << ' '; });

    cout << endl;

    for_each_n(begin(data), 5, [](auto value) { cout << value << ' '; });
}

template <typename T>
void tell_type() {
    if (is_void<T>::value) {
        cout << "T 는 void ! \n";
    }
    else {
        cout << "T 는 void 가 아니다. \n";
    }
}

void TypeTraitTest() {
    tell_type<int>();  // void 아님!

    tell_type<void>();  // void!
}

void Int32Int64() {
    // dword, eax
    int a32 = 1;
    int b32 = 1;
    int c32 = a32 + b32;
    // qword,rax
    __int64 a64 = 1;
    __int64 b64 = 1;
    __int64 c64 = a64 + b64;
}

void print(int a) {
    printf("%d\n", a);
}

struct Print {
    void operator()(int a) const {
        printf("%d\n", a);
    }
};

void FunctorTest() {
    int ari[] = { 2,8,5,1,9 };
    for_each(ari, ari + 5, print);  // 함수
    for_each(ari, ari + 5, Print());// 함수객체
    for_each(ari, ari + 5, [](int a) { printf("%d\n", a); });// 람다
}

void RValueRefTest() {
    int a = 1;
    int& la = a;
    //int&& ra = a;

    //int& lb = 2;
    int&& rb = 2;
    rb = 4;
    auto p = &rb;
}

void DefaultDeleterTest() {
    shared_ptr<int> p(new int);
    shared_ptr<int> p1(new int, default_delete<int>());
    shared_ptr<int> parr(new int[100], default_delete<int[]>());
}

void SharedPtrTest2() {
    shared_ptr<int> a1(new int(1));
    auto b = shared_ptr<int>(new int(1));
    auto c = make_shared<int>(1);
    auto a2 = a1;
    auto a3 = a1;
    auto a4 = a1;
    auto a5 = a1;
    wcout << L"a1 : " << a1 << ":" << a1.use_count() << endl;
    wcout << L"a2 : " << a2 << ":" << a2.use_count() << endl;
    wcout << L"a3 : " << a3 << ":" << a3.use_count() << endl;
    wcout << L"a4 : " << a4 << ":" << a4.use_count() << endl;
    wcout << L"a5 : " << a5 << ":" << a5.use_count() << endl;
}

void VariableInitialzeTest() {
    int i1 = 1;
    int i2(1);
    auto i3 = 1;
    auto i4(1);
    const char* c1 = "aaa";
    const char* c2("aaa");
    auto c3 = "aaa";
    auto c4("aaa");
}

class AA {
public:
    AA() {
        wcout << this << " : AA()" << endl;
    }
    AA(const AA& rhs) {
        wcout << this << " : AA(const AA& rhs)" << endl;
    }
    AA& operator=(const AA& rhs) {
        wcout << this << " : operator=(const AA& rhs)" << endl;
        return *this;
    }
    ~AA() {
        wcout << this << " : ~AA()" << endl;
    }
};

class BB {
public:
    int a;
    AA aa;

    BB() {
        wcout << this << " : BB()" << endl;
    }
    BB(const BB& rhs) {
        wcout << this << " : BB(const BB& rhs)" << endl;
    }
    BB& operator=(const BB& rhs) {
        aa = rhs.aa;
        wcout << this << " : operator=(const BB& rhs)" << endl;
        return *this;
    }
    ~BB() {
        wcout << this << " : ~BB()" << endl;
    }
};

void ClassTest() {
    BB b;
    BB b2 = b;
    b2 = b;
}

void LambdaTest3() {
    auto addFunc = [](int a, int b) { return a + b; };
    wcout << addFunc(3, 4) << endl;
    // 1. function<int(int, int)> addFunc : 람다 변수 선언
    // -> 선언과 동시에 초기화 시 function addFunc 로 간단화 가능
    // -> 선언과 동시에 초기화 시 auto addFunc 로 간단화 가능
    // 2. [num] : 캡쳐
    // -> [num] : num변수 const 복사 캡쳐
    // -> [&num] : num변수 참조 캡쳐
    // -> [=] : 모든변수 const 복사 캡쳐
    // -> [&] : 모든변수 참조 캡쳐
    // 3. mutable : 복사 캡쳐 변수를 비 const화
    // 4. (int a, int b) : 파라미터 변수
    // -> 파라미터 없을때 [생략 가능]
    // 5. -> int : 리턴 타입
    // -> 본문에 의해 추론 가능 할때 [생략 가능]
    // 6. {} 본문
}

void ShredPtrTest3() {
    // 1. 선언 방법
    auto pbyte1 = shared_ptr<BYTE>(new BYTE);
    shared_ptr<BYTE> pbyte2(new BYTE);
    // 2. deleter
    shared_ptr<BYTE> pbyte3(new BYTE, [](BYTE *ptr) { delete ptr; });
    shared_ptr<BYTE> arrbyte1(new BYTE[10], [](BYTE *ptr) { delete[] ptr; });
    // 3. default_deleter
    shared_ptr<BYTE> pbyte4(new BYTE, default_delete<BYTE>());
    shared_ptr<BYTE> arrbyte2(new BYTE[10], default_delete<BYTE[]>());

    unique_ptr<BYTE> pbyte5(new BYTE);
    unique_ptr<BYTE> arrbyte3(new BYTE[10]);

    // not legal, operator[] not defined
    //pbyte4[1] = 4;
    //arrbyte2[1] = 4;
    //pbyte5[1] = 4;
    //arrbyte3[1] = 4;

    //auto shared = std::make_shared<char[]>(64);   // not legal, (legal form c++20)
    auto unique = std::make_unique<char[]>(64);
}

void SharedPtrTest4_sub(unique_ptr<int> a) {
    wcout << a << endl;
}

void SharedPtrTest4() {
    unique_ptr<int> a(new int);
    *a = 10;
    // SharedPtrTest4_sub(a);   // 복사생성 불가
    // auto b = a;  // 복사생성 불가 delete 되었음
    //// unique_ptr(const unique_ptr&) = delete;
    //// unique_ptr& operator=(const unique_ptr&) = delete;
    
    auto b = move(a);
    *a = 20;    // 런타임 에러
}

void VectorTest5() {
    vector<int> vec = { 1, 2, 3 };
    vec[1] = 10;
    for (auto n : vec) {
        wcout << n << endl;
    }
}

std::vector<size_t> GenRange(size_t start, size_t num) {
    std::vector<size_t> vec(num);
    std::iota(vec.begin(), vec.end(), start);
    return vec;
}

void ParallelForeachTest() {
    auto range = GenRange(0, 100);

    //std::for_each(seq.begin(), seq.end(), [](auto num) { cout << num << " "; });

    //auto policy = std::execution::seq;
    //auto policy = std::execution::par;
    auto policy = std::execution::par_unseq;

    std::mutex m;
    std::for_each(policy, range.begin(), range.end(), [&](auto num) {
        //std::lock_guard<std::mutex> guard(m);
        m.lock();
        cout << num << std::endl;
        m.unlock();
                  });
}

void ThreadFunc(int repeatNum, bool hasError) {
    int *ptr = nullptr;
    for (int i = 0; i < repeatNum; i++) {
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::sleep_for(chrono::milliseconds(10));
        if (hasError && i == repeatNum / 2) {
            *ptr = 10;
        }
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::sleep_for(chrono::milliseconds(10));
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void ThreadTest() {
    const int threadNum = 4;
    int taskNum = 1000;
    int repeatNum = taskNum / threadNum;
    thread ts[threadNum];
    for (int i = 0; i < threadNum; i++) {
        ts[i] = thread(ThreadFunc, repeatNum, (i == 2));
        cout << "thread start : " << i << endl;
    }
    for (int i = 0; i < threadNum; i++) {
        ts[i].join();
    }
    cout << "all thread finished" << endl;
}

class AAA {
public:
    static void Func1() {
        cout << "Func1" << endl;
        // 클래스 함수에서는 뒤에 정의된 클래스 함수도 호출 가능하다.
        Func2();
    }

    static void Func2() {
        cout << "Func2" << endl;
    }
};

void Func3() {
    cout << "Func3" << endl;
    // 일반 함수는 뒤에 정의된 함수 호출 불가
    //Func4();
}

void Func4() {
    cout << "Func4" << endl;
}

void ClassMemberOrder() {
    AAA::Func1();
    Func3();
}

void AssignTest() {
    int a = 1;
    int b(2);
    int c{ 3 };

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
}

class Cls1 {
private:
    int a;
public:
    Cls1(int a) : a{ a } {};
};

// const int a = 1; int const a = 1; 동일
// const int * a = &v; int const * a = &v; 동일
// const int * a = &v; int * const a; 상수 int, 상수 a
// 상수 객체는 상수 멤버 함수만 호출 가능
// 상수 멤버 함수는 상수 멤버 함수만 호출 가능

void ConstTest() {
    const int i1 = 10;
    int const i2 = 10;  // 위의 t1, t2는 동일함, i1, i2 상수, 변수값 변경 불가

    int a = 10;
    const int * p1 = &a;
    int const * p2 = &a; // 위의 p1, p2는 동일함, *p1, *p2 상수, 포인터값 변경 가능, 포인터가 가리키는 값 변경 불가
    // p2 = NULL;   // O
    // *p2 = 10;    // X
    
    int * const p3 = &a; // p3 상수, 포인터값 변경 불가, 포인터가 가리키는 값 변경 가능
    // p3 = NULL;   // X
    // *p3 = 10;    // O
}

class Cls2 {
private:
    BYTE buffer[1024];
    int len = sizeof(buffer);
    BYTE * ptr;
public:
    void AAA() {
        BYTE* a = GetPtr();         // GetPtr() 호출됨
        *a = 10;
    }
    void BBB() const {
        const BYTE* a = GetPtr();   // GetPtr() const 호출됨
        //*a = 10; 
    }

    BYTE* GetPtr() {
        return buffer;
    }
    BYTE* GetPtr1() {
        return buffer;
    }
    void CCC() const {
        auto t = GetPtr();
    }
    const BYTE* GetPtr() const {    // 상수객체에서는 상수멤버함수만 호출됨, 상수멤버함수에서는 상수멤버함수만 호출됨
        return buffer;
    }
    const BYTE* GetPtr2() const {    // 상수객체에서는 상수멤버함수만 호출됨, 상수멤버함수에서는 상수멤버함수만 호출됨
        return buffer;
    }
};

void RangeTest() {
    using namespace std::views;
    using namespace std::ranges;
    auto r =
        std::views::iota(0, 20)
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return -n; });
    std::ranges::for_each(r, [](int n) { cout << n << " "; });

    //// C# euqivalent
    //var r =
    //    Enumerable.Range(0, 20)
    //    .Where(n => n % 2 == 0)
    //    .Select(n => -n);
    //r.ToList<int>().ForEach(n => Console.Write("{0} ", n));
}

void StringEncodingTest() {
    string str = "abcd가나다라";
    wstring wstr = L"abcd가나다라";
    u8string u8str = u8"abcd가나다라";
    u16string u16str = u"abcd가나다라";
    u32string u32str = U"abcd가나다라";

    cout << "sizeof(char) : " << sizeof(char) << endl;            // 1
    cout << "sizeof(wchar_t) : " << sizeof(wchar_t) << endl;      // 2
    cout << "sizeof(char8_t) : " << sizeof(char8_t) << endl;      // 1
    cout << "sizeof(char16_t) : " << sizeof(char16_t) << endl;    // 2
    cout << "sizeof(char32_t) : " << sizeof(char32_t) << endl;    // 4

    cout << "str.length() : " << str.length() << endl;            // 12   -> 바이트수
    cout << "wstr.length() : " << wstr.length() << endl;          // 8    -> 문자수
    cout << "u8str.length() : " << u8str.length() << endl;        // 16   -> 바이트수
    cout << "u16str.length() : " << u16str.length() << endl;      // 8    -> 문자수
    cout << "u32str.length() : " << u32str.length() << endl;      // 8    -> 문자수

    cout << "str.capacity() : " << str.capacity() << endl;
    cout << "wstr.capacity() : " << wstr.capacity() << endl;
    cout << "u8str.capacity() : " << u8str.capacity() << endl;
    cout << "u16str.capacity() : " << u16str.capacity() << endl;
    cout << "u32str.capacity() : " << u32str.capacity() << endl;

    cout << "str : " << str << endl;
    //cout << "wstr : " << wstr << endl;
    //cout << "u8str : " << u8str << endl;    // 안됨
    //cout << "u16str : " << u16str << endl;  // 안됨
    //cout << "u32str : " << u32str << endl;  // 안됨

    //wcout << "str : " << str << endl;
    wcout << "wstr : " << wstr << endl;
    //wcout << "u8str : " << u8str << endl;   // 안됨
    //wcout << "u16str : " << u16str << endl; // 안됨
    //wcout << "u32str : " << u32str << std::endl; // 안됨

    //wstring str;
}

//void Pointer_of_SharedPtr_Test() {
//    auto car = make_shared<Car>("아반떼");
//    auto pcar = &car;
//    auto a = pcar->get();
//    auto b = (*pcar).get();
//    auto c = (*pcar)->fuel;
//}

void StringEncodingTest2() {
    cout << "sizeof(char_t) : " << sizeof(char) << endl;
    cout << "sizeof(wchar_t) : " << sizeof(wchar_t) << endl;
    cout << "sizeof(char8_t) : " << sizeof(char8_t) << endl;
    cout << "sizeof(char16_t) : " << sizeof(char16_t) << endl;
    cout << "sizeof(char32_t) : " << sizeof(char32_t) << endl;
    cout << endl;

    string str_ENG = "A";
    wstring wstr_ENG = L"A";
    u8string u8str_ENG = u8"A";
    u16string u16str_ENG = u"A";
    u32string u32str_ENG = U"A";

    string str_KOR = "가";
    wstring wstr_KOR = L"가";
    u8string u8str_KOR = u8"가";
    u16string u16str_KOR = u"가";
    u32string u32str_KOR = U"가";

    string str_SMP = "𝛃";
    wstring wstr_SMP = L"𝛃";
    u8string u8str_SMP = u8"𝛃";
    u16string u16str_SMP = u"𝛃";
    u32string u32str_SMP = U"𝛃";

    cout << "str_ENG.length() : " << str_ENG.length() << endl;
    cout << "wstr_ENG.length() : " << wstr_ENG.length() << endl;
    cout << "u8str_ENG.length() : " << u8str_ENG.length() << endl;
    cout << "u16str_ENG.length() : " << u16str_ENG.length() << endl;
    cout << "u32str_ENG.length() : " << u32str_ENG.length() << endl;
    cout << endl;

    cout << "str_KOR.length() : " << str_KOR.length() << endl;
    cout << "wstr_KOR.length() : " << wstr_KOR.length() << endl;
    cout << "u8str_KOR.length() : " << u8str_KOR.length() << endl;
    cout << "u16str_KOR.length() : " << u16str_KOR.length() << endl;
    cout << "u32str_KOR.length() : " << u32str_KOR.length() << endl;
    cout << endl;

    cout << "str_SMP.length() : " << str_SMP.length() << endl;
    cout << "wstr_SMP.length() : " << wstr_SMP.length() << endl;
    cout << "u8str_SMP.length() : " << u8str_SMP.length() << endl;
    cout << "u16str_SMP.length() : " << u16str_SMP.length() << endl;
    cout << "u32str_SMP.length() : " << u32str_KOR.length() << endl;
    cout << endl;

    /* run result (VC)
sizeof(char_t) : 1
sizeof(wchar_t) : 2
sizeof(char8_t) : 1
sizeof(char16_t) : 2
sizeof(char32_t) : 4

str_ENG.length() : 1
wstr_ENG.length() : 1
u8str_ENG.length() : 1
u16str_ENG.length() : 1
u32str_ENG.length() : 1

str_KOR.length() : 2
wstr_KOR.length() : 1
u8str_KOR.length() : 3
u16str_KOR.length() : 1
u32str_KOR.length() : 1

str_SMP.length() : 2
wstr_SMP.length() : 2
u8str_SMP.length() : 4
u16str_SMP.length() : 2
u32str_SMP.length() : 1
    */

    /* run result (GNU C++20)
sizeof(char_t) : 1
sizeof(wchar_t) : 4
sizeof(char8_t) : 1
sizeof(char16_t) : 2
sizeof(char32_t) : 4

str_ENG.length() : 1
wstr_ENG.length() : 1
u8str_ENG.length() : 1
u16str_ENG.length() : 1
u32str_ENG.length() : 1

str_KOR.length() : 3
wstr_KOR.length() : 1
u8str_KOR.length() : 3
u16str_KOR.length() : 1
u32str_KOR.length() : 1

str_SMP.length() : 4
wstr_SMP.length() : 1
u8str_SMP.length() : 4
u16str_SMP.length() : 2
u32str_SMP.length() : 1
    */
}

void PredefinedFunctionObject() {
    cout << "plus<int>()(10, 3) : " << plus<int>()(10, 3) << endl;
    cout << "minus<int>()(10, 3) : " << minus<int>()(10, 3) << endl;
    cout << "multiplies<int>()(10, 3) : " << multiplies<int>()(10, 3) << endl;
    cout << "divides<int>()(10, 3) : " << divides<int>()(10, 3) << endl;
    cout << "modulus<int>()(10, 3) : " << modulus<int>()(10, 3) << endl;
    cout << "negate<int>()(10) : " << negate<int>()(10) << endl;
    cout << "equal_to<int>()(10, 3) : " << equal_to<int>()(10, 3) << endl;
    cout << "not_equal_to<int>()(10, 3) : " << not_equal_to<int>()(10, 3) << endl;
    cout << "greater<int>()(10, 3) : " << greater<int>()(10, 3) << endl;
    cout << "less<int>()(10, 3) : " << less<int>()(10, 3) << endl;
    cout << "greater_equal<int>()(10, 3) : " << greater_equal<int>()(10, 3) << endl;
    cout << "less_equal<int>()(10, 3) : " << less_equal<int>()(10, 3) << endl;
    cout << "logical_and<bool>()(true, false) : " << logical_and<bool>()(true, false) << endl;
    cout << "logical_or<bool>()(true, false) : " << logical_or<bool>()(true, false) << endl;
    cout << "logical_not<bool>()(true) : " << logical_not<bool>()(true) << endl;
    cout << "bit_and<int>()(10, 3) : " << bit_and<int>()(10, 3) << endl;
    cout << "bit_or<int>()(10, 3) : " << bit_or<int>()(10, 3) << endl;
    cout << "bit_xor<int>()(10, 3) : " << bit_xor<int>()(10, 3) << endl;
    cout << "bit_not<int>()(10) : " << bit_not<int>()(10) << endl;
}

void ConsoleColor() {
    cout << "\033[0;31m" << "red" << endl;
    cout << "\033[1;31m" << "red" << endl;
    cout << "\033[2;31m" << "red" << endl;
    cout << "\033[3;31m" << "red" << endl;
    cout << "\033[4;31m" << "red" << endl;
    cout << "\033[1;32m" << "green" << endl;
    cout << "\033[1;33m" << "yellow" << endl;
    cout << "\033[1;36m" << "cyan" << endl;
    cout << "\033[1;35m" << "magenta" << endl;
    cout << "\033[0m" << "reset" << endl;
}

void ConsoleColor2() {
    int i, j, n;

    for (i = 0; i < 11; i++) {
        for (j = 0; j < 10; j++) {
            n = 10 * i + j;
            if (n > 108) break;
            printf("\033[%dm %3d\033[m", n, n);
        }
        printf("\n");
    }
}

int main() {
    //VariableArgumentTest();
    //OpenMPTest();
    //DuckTypingTest();
    //RangeForTest();
    //StringFormatTest();
    //PrintfTest();
    //MemoryLeakTest();
    //NullPtrTest();
    //SharedPtrTest();
    //SharedPtrInitTest();
    //SharedPtrVectorTest();
    //VectorTest();
    //VectorTest2();
    //VectorTest3();
    //VectorTest4();
    //VectorTestString();
    //ChronoTestDuration();
    //ChronoTestNow();
    //ObjectReturnTest();
    //PromiseFutureTest();
    //ConstPointerTest();
    //StaticMemberTest();
    //CarSetTest();
    //GraphicSetTest();
    //StringConvertTest();
    //StringConvertTest2();
    //RangeBasedForTest();
    //MapTest();
    //ForTest();
    //RemainderTest();
    //VectorSortTest();
    //ReferenceTest();
    //StructInitialize();
    //IteratorTest();
    //StlFindTest();
    //StlSortTest();
    //StlReverseTest();
    //StlRandomShuffleTest();
    //SomeMesmerizeTest1();
    //VectorElementAccessTest();
    //ListElementAccessTest();
    //ForeachFunctorTest();
    //FindIfTest();
    //PredefFunctorTest();
    //StringSortNoCaseTest();
    //VectorSortSpeedTest();
    //RoundTest();
    //NegativeModulusTest();
    //SizeOfPrimitiveTest();
    //FillTest();
    //GenerateTest();
    //IotaTest();
    //TransformTest();
    //InheritConstructorTest();
    //TryCatchTest();
    //LambdaTest1();
    //LambdaTest2();
    //StringAppendTest();
    //MoveConstructorOperator();
    //RefTest();
    //DebugViewTest();
    //QuickSortTest();
    //ForEachTest();
    //TypeTraitTest();
    //Int32Int64();
    //FunctorTest();
    //RValueRefTest();
    //DefaultDeleterTest();
    //SharedPtrTest2();
    //ClassTest();
    //LambdaTest3();
    //ShredPtrTest3();
    //MoveTest();
    //VectorTest5();
    //ParallelForeachTest();
    //ThreadTest();
    //ClassMemberOrder();
    //AssignTest();
    //SharedPtrTest4();
    //RangeTest();
    //tringEncodingTest2();
    //PredefinedFunctionObject();
    ConsoleColor();
    ConsoleColor2();

    return 0;
}