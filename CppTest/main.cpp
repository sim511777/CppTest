#include <stdio.h>
#include <ppl.h>
#include <string.h>
#include <tchar.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <locale>
#include <codecvt>
#include <windows.h>
#include <algorithm>
#include <list>

using namespace std;
using namespace concurrency;
using namespace chrono;

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

void VariableArgumentTest() {
    cout << "==== VariableArgumentTest ====" << endl << endl;
    int iSum = IntSum(3, 1, 2, 3);
    printf("int sum = %d\n", iSum);
    double dSum = DoubleSum(3, 1.1, 2.2, 3.3);
    printf("double sum = %f\n", dSum);
}

void PplTest() {
    cout << "==== PplTest ====" << endl << endl;
    printf("normal\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", i);
    }
    printf("\npareallel_for\n");
    // start로부터 시작해서 last가 아닌 동안 한번 할때마다 step 씩 증가
    parallel_for(0, 10, 1, [&](int i) {
        printf("%d ", i);
        });
}

void OpenMPTest() {
    cout << "==== OpenMPTest ====" << endl << endl;
    //#pragma omp parallel
    //{
    //    printf("Hello, OpenMP\n");
    //    printf("Bye, world\n");
    //}

    //int sum = 0;
    //#pragma omp parallel for
    //for (int i = 0; i < 10; i++) {
    //    printf("i = %d\n", i);
    //    sum += i;
    //}
    //printf("Sum = %d\n", sum);
}

class Duck {
public:
    void quack() {
        printf("Quaaaaaack!\n");
    }

    void feathers() {
        printf("The duck has white and gray feathers.\n");
    }
};

class Person {
public:
    void quack() {
        printf("The person imitates a duck.\n");
    }

    void feathers() {
        printf("The person takes a feather from the ground and shows it.\n");
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
        printf("%d\n", i);
}

void StringFormatTest() {
    cout << "==== StringFormatTest ====" << endl << endl;
    ostringstream oss;
    oss << " my age : " << 16;
    oss << " my height : " << 170;
    auto str = oss.str();
    auto cp = str.c_str();
    printf(cp);
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
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 할당 번호 {165} 를 아래에 함수에 넣어주면 해당 할당시 런타임 에러를 발생시켜 준다. 
    _CrtSetBreakAlloc(165);

    auto a = new int[10];
    auto b = new int[10];
    auto c = new int[20];

    delete[] a;
    delete[] c;
}

void UnrefFunctionList() {
    // Project 속성에서
    // C/C++ -> 코드생성 -> 함수수준링크사용 : 예(/Gy)
    // 링커 -> 최적화 -> 참조 : Yes(/OPT:REF)
    // 링커 -> 명령줄 -> 추가옵션 : /VERBOSE:REF 추가
}

void Func(int a) { printf("정수를 인자로 받는 함수\n"); }
void Func(int* a) { printf("포인터를 인자로 받는 함수\n"); }
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

class Graphic {
public:
    Graphic() { puts("그래픽 생성"); }
    virtual void Draw() { puts("그래픽 오브젝트입니다."); }
};

class Line : public Graphic {
public:
    Line() { puts("선 생성"); }
    void Draw() { puts("선을 긋습니다."); }
};

class Circle : public Graphic {
public:
    Circle() { puts("원 생성"); }
    void Draw() { puts("동그라미 그렸다 치고."); }
};

class Rect : public Graphic {
public:
    Rect() { puts("사각형 생성"); }
    void Draw() { puts("요건 사각형입니다."); }
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
    //   printf("%d\n", i);
    for_each(intlist.begin(), intlist.end(), [](int a) { printf("%d\n", a); });

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
    //   printf("%d\n", i);
    for_each(intlist.begin(), intlist.end(), [](int a) { printf("%d\n", a); });

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
        printf("%d\n", intlist[i]);
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
        printf("%s\n", (*it).c_str());
    }
}

void ChronoTestDuration() {
    cout << "==== ChronoTestDuration ====" << endl << endl;
    time_point<system_clock> t0 = system_clock::now();
    this_thread::sleep_for(seconds(3));
    system_clock::time_point t1 = system_clock::now();
    duration<double> sec = t1 - t0;
    printf("%f\n", sec.count());
}

void ChronoTestNow() {
    cout << "==== ChronoTestNow ====" << endl << endl;
    time_point<system_clock> now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    char str[26];
    ctime_s(str, sizeof str, &t);
    printf("%s\n", str);
}

struct MyCar {
    int speed;
    int fule;
    int direction;
};

MyCar GetMyCar() {
    MyCar car;
    printf("object from callee : %p\n", &car);
    return car;
}

void ObjectReturnTest() {
    cout << "==== ObjectReturnTest ====" << endl << endl;
    MyCar car = GetMyCar();
    printf("object from caller : %p\n", &car);
}

int fun() {
    for (int i = 1; i <= 10; i++) {
        printf("fun[%d]\n", i);
    }

    return 200;
}

void PromiseFutureTest() {
    cout << "==== PromiseFutureTest ====" << endl << endl;
    //future<int> fut = async(fun);
    future<int> fut = async(launch::async, fun);  // 비동기로 실행
    //future<int> fut = async(launch::deferred, fun);

    for (int i = 1; i <= 10; i++) {
        printf("main[%d]\n", i);
    }
    printf("\n");

    int result = fut.get();   // 비동기 작업이 완료될때까지 대기하고 리턴값 받음

    printf("result : %d\n", result);
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

wstring StrToWstr(string const& str)
{
    size_t len = str.size();
    size_t bufSize = len + 1;
    wchar_t* wc = new wchar_t[bufSize];
    size_t lenDone;
    mbstowcs_s(&lenDone, wc, bufSize, str.c_str(), len);
    wstring wstr(wc);
    delete[] wc;
    return wstr;
}

string WstrToStr(wstring const& wstr)
{
    size_t len = wstr.size();
    size_t bufSize = len * 2 + 1;
    char* c = new char[bufSize];
    size_t lenDone;
    wcstombs_s(&lenDone, c, bufSize, wstr.c_str(), len);
    string str(c);
    delete[] c;
    return str;
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

    string _6_str3 = string(_3_wstr.begin(), _3_wstr.end());
    wstring _7_wstr3 = wstring(_2_str.begin(), _2_str.end());

    cout << "char* : " << _0_ch << endl;
    wcout << "wchar_t* : " << _1_wch << endl;

    cout << "string : " << _2_str << endl;
    wcout << L"wstring : " << _3_wstr << endl;

    cout << "WstrToStr : " << _4_str2 << endl;
    wcout << L"StrToWstr : " << _5_wstr2 << endl;

    cout << "string(iter, iter) : " << _6_str3 << endl;
    wcout << L"wstring(iter, iter) : " << _7_wstr3 << endl;
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

void StringConvertTest2() {
    cout << "==== StringConvertTest2 ====" << endl << endl;
    //locale::global(locale("ko-KR"));
    //setlocale(LC_ALL, "ko-KR");

    const char* _0_ch = "안녕AB";
    const wchar_t* _1_wch = L"안녕AB";

    string  _2_str = "안녕AB";
    wstring _3_wstr = L"안녕AB";

    string _4_str2 = wcs_to_mbs(_3_wstr);
    wstring _5_wstr2 = mbs_to_wcs(_2_str);

    string _6_str3 = string(_3_wstr.begin(), _3_wstr.end());
    wstring _7_wstr3 = wstring(_2_str.begin(), _2_str.end());

    cout << "char* : " << _0_ch << endl;
    wcout << "wchar_t* : " << _1_wch << endl;

    cout << "string : " << _2_str << endl;
    wcout << L"wstring : " << _3_wstr << endl;

    cout << "WstrToStr : " << _4_str2 << endl;
    wcout << L"StrToWstr : " << _5_wstr2 << endl;

    cout << "string(iter, iter) : " << _6_str3 << endl;
    wcout << L"wstring(iter, iter) : " << _7_wstr3 << endl;
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
        printf("%d / 2 = %d, %d %% 2 = %d\n", i, i / 2, i, i % 2);
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
    vector<int> intList { 2, 9, 1, 5, 4, 7, 6, 3, 0, 8,  };
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

ostream& operator <<(ostream& c, const RECT& T) {
    return c << "{" << T.left << "," << T.top << "," << T.right << "," << T.bottom << "}";
}

void ReferenceTest() {
    cout << "==== ReferenceTest ====" << endl << endl;
    RECT rect1 = {1,2,3,4,};
    RECT rect2 = {11,12,13,14};
    RECT & refRect = rect1;
    cout << rect1 << " " << rect2 << " " << refRect << endl;
    refRect = rect2;
    cout << rect1 << " " << rect2 << " " << refRect << endl;
}

void StructInitialize() {
    cout << "==== StructInitialize ====" << endl << endl;
    RECT rect1;
    rect1.left = 1;
    rect1.top = 2;
    rect1.right = 3;
    rect1.bottom = 4;

    RECT rect2 = { 1,2,3,4, };
    RECT rect3{ 1,2,3,4 };
}

template<typename IT>
void PrintIterator(IT s, IT e) {
    IT it;
    for (it = s; it != e; it++) {
        printf("%d ", *it);
    }
}

template<typename T>
void PrintIndex(T seq, int s, int e) {
    int i;
    for (i = s; i != e; i++) {
        printf("%d ", seq[i]);
    }
}

void IteratorTest() {
    cout << "==== IteratorTest ====" << endl << endl;
    
    int ari[] = { 1,2,3,4,5 };
    vector<int> vi(&ari[0], &ari[5]);
    list<int> li(&ari[0], &ari[5]);

    cout << endl << "iterate array" << endl;
    PrintIterator(&ari[0], &ari[5]);
    cout << endl << "iterate vector" << endl;
    PrintIterator(vi.begin(), vi.end());
    cout << endl << "iterate list" << endl;
    PrintIterator(li.begin(), li.end());

    cout << endl << "index array" << endl;
    PrintIndex(ari, 0, 5);
    cout << endl << "index vector" << endl;
    PrintIndex(vi, 0, vi.size());
    // error : list는 인덱싱 연산자([])가 정의되어 있지 않다.
    //cout << endl << "index list" << endl;
    //PrintIndex(li, 0, li.size());
}

void StlFindTest() {
    cout << "==== StlFindTest ====" << endl << endl;
    
    int arr[] = { 1,2,3,4,5 };
    vector<int> vi = { 1,2,3,4,5 };
    list<int> li = { 1,2,3,4,5 };

    puts(find(vi.begin(), vi.end(), 4) == vi.end() ? "없다." : "있다.");
    puts(find(li.begin(), li.end(), 8) == li.end() ? "없다." : "있다.");
    puts(find(&arr[0], &arr[5], 3) == &arr[5] ? "없다." : "있다.");
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
    MyMesmerize obj();  // 햇갈리지 말자. MyMesmerize객체 생성이 아니고 MyMesmerize를 리턴하는 파라미터 없는 obj함수 선언. 아무것도 안함.
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

    int a = intList.at(3);  // get
    int b = intList[4];     // get
    cout << a << endl;
    cout << b << endl;

    intList.at(3) = 8;      // set
    intList[4] = 9;         // set
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

    list<int>::iterator it = intList.begin();
    advance(it, 3);  // get
    int a = *it;
    cout << a << endl;

    *it = 8;      // set
    for (int i : intList)
        cout << i << " ";
    cout << endl;
}

int main() {
    //VariableArgumentTest();
    //PplTest();
    //OpenMPTest();
    //DuckTypingTest();
    //RangeForTest();
    //StringFormatTest();
    //PrintfTest();
    //MemoryLeakTest();
    //NullPtrTest();
    //SharedPtrTest();
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
    ListElementAccessTest();

    getchar();
    return 0;
}