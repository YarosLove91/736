#include <iostream>
using std::cout;
using std::endl;

#include <utility>
#include <vector>
using std::vector;

#include <thread>
#include <string>
using std::string;
using std::wstring;

#include <mutex>
using std::mutex;
using std::lock_guard;

#include <future>
using std::promise;
using std::future;

#include <functional>
#include <random>
#include <string>
#include <sstream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"

#include <Windows.h>
#include <iterator>

using std::mem_fn;

void set_color(int text, int background)
{
    static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss<< val;
    return oss.str();
}

template<typename T>
T fromString(const std::string& s)
{
    std::istringstream iss(s);
    T res;
    iss >> res;
    return res;
}

struct TestPromiseStruct {
public:
        size_t _delay {0};
        std::string _string {};

};


class Modifier;

std::string printMessage (const string & message, unsigned delayTime ){

    const std::string & printMessage = const_cast<string &> (message);
    // Преобразовываем в строку текущий номер потока и время задержки
    std::string myString {"printMessage thread ID: "};
    // Состав строки: printMessage thread ID:  + номер потока + Delay thread: +  mSec + сообщение
    myString.append(toString(std::this_thread::get_id())).append(" Delay thread: ").append(toString(delayTime)).append(" mSec\n");
    myString.append(printMessage).append("\n");
    std::this_thread::sleep_for (std::chrono::milliseconds(delayTime));
    return myString;
}


int main() {

    cout <<"Main thread ID: "<< std::this_thread::get_id() << endl;

    // Вихрь Мерсенна
    std::mt19937 gen;
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution <int> int_dist(0, 5000);              // Для задержки потока

    size_t totalDelay {0};
    TestPromiseStruct DataTransferStruct;

    std::vector<string> textList;
    textList.reserve(10);

    textList.emplace_back("The old lady pulled her spectacles down and looked over them about the room; "
                            "then she put them up and looked out under them. She seldom or never looked through them for so small a thing as a boy; "
                            "they were her state pair, the pride of her heart, and were built for “style,” "
                            "not service—she could have seen through a pair of stove-lids just as well. She looked perplexed for a moment, "
                            "and then said, not fiercely, but still loud enough for the furniture to hear:");
    textList.emplace_back("Well, I lay if I get hold of you I`ll-");
    textList.emplace_back("She did not finish, for by this time she was bending down and punching under the bed with the broom, "
                            "and so she needed breath to punctuate the punches with. She resurrected nothing but the cat.");
    textList.emplace_back("I never did see the beat of that boy!");
    textList.emplace_back("She went to the open door and stood in it and looked out among the tomato vines and “jimpson” weeds that constituted the garden. "
                            "No Tom. So she lifted up her voice at an angle calculated for distance and shouted:");
    textList.emplace_back("Y-o-u-u TOM!");
    textList.emplace_back("There was a slight noise behind her and she turned just in time to seize "
                            "a small boy by the slack of his roundabout and arrest his flight.");

    textList.shrink_to_fit();

    // Контейнер с потоками
    std::vector <std::future <std::string>> tasks;
    // Резервирую элементы вектора
    tasks.reserve(textList.size());

    auto start = std::chrono::high_resolution_clock::now();                      // Для расчета времени исполнения программы

    // Запускаем пачку задачек
    // каждая задачка перемещает в себя строку и и получает случайное значение задежки
    for (auto & item : textList){
        // Перемещаю строку из списка и помещаю задачу в вектор
        // автоматически создаем поток
        size_t currentDelay = int_dist(gen);            // Задержка
        totalDelay += currentDelay;                        // Суммарная получившаяся задержка
        tasks.emplace_back(std::async (std::launch::async | std::launch::deferred, printMessage, std::move (item), currentDelay));
    }
    // Поскольку из вектора уже перенесли все значения

    textList.clear();

    // Перебираю все задачи на предмет получения данных и завершения
    // у лямбды захват по ссылке
    std::for_each(tasks.begin(),tasks.end(), [&]( std::future <std::string> & returnString){
        cout << returnString.get() << endl;
    });          // Ждемс, пока все задачи завершаться


    auto end = std::chrono::high_resolution_clock::now();

    // Calculating total time taken by the program.
    double time_taken =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    cout << "Time taken by program is : " << std::fixed << time_taken << '\n' <<
    "Total delay time: " << totalDelay << " mSec" << endl;

    // Уничтожаем список и список задач
    tasks.clear();
    totalDelay = 0;

    // Вариант 2. С лямбдочкой
    // #########################################################################################################
    system("COLOR C");
    cout << "Chapter 2" << endl;
    system("COLOR 0");

    // Заново инициализируем вектор списком сообщений
    textList.emplace_back("The old lady pulled her spectacles down and looked over them about the room; "
                          "then she put them up and looked out under them. She seldom or never looked through them for so small a thing as a boy; "
                          "they were her state pair, the pride of her heart, and were built for 'style,' "
                          "not service—she could have seen through a pair of stove-lids just as well. She looked perplexed for a moment, "
                          "and then said, not fiercely, but still loud enough for the furniture to hear:");
    textList.emplace_back("Well, I lay if I get hold of you I`ll-");
    textList.emplace_back("She did not finish, for by this time she was bending down and punching under the bed with the broom, "
                          "and so she needed breath to punctuate the punches with. She resurrected nothing but the cat.");
    textList.emplace_back("I never did see the beat of that boy!");
    textList.emplace_back("She went to the open door and stood in it and looked out among the tomato vines and “jimpson” weeds that constituted the garden. "
                          "No Tom. So she lifted up her voice at an angle calculated for distance and shouted:");
    textList.emplace_back("Y-o-u-u TOM!");
    textList.emplace_back("There was a slight noise behind her and she turned just in time to seize "
                          "a small boy by the slack of his roundabout and arrest his flight.");


    // Пораждаю лямбдочку
    auto myLambda = ([] (const string& message, unsigned delayTime)
    {
        const std::string & printMessage = const_cast<string &> (message);
        std::string myString {"printMessage thread Lambd_ID: "};
        // Состав строки: printMessage thread ID:  + номер потока + Delay thread: +  mSec + сообщение
        myString.append(toString(std::this_thread::get_id())).append(" Delay thread: ").append(toString(delayTime)).append(" mSec\n");
        myString.append(printMessage).append("\n");
        std::this_thread::sleep_for (std::chrono::milliseconds(delayTime));
        return myString;
    });

    start = std::chrono::high_resolution_clock::now();
    for (auto & item : textList){
        // Перемещаю строку из списка и помещаю задачу в вектор
        // автоматически создаем поток
        size_t currentDelay = int_dist(gen);            // Задержка
        totalDelay += currentDelay;                        // Суммарная получившаяся задержка
        // Запускаю поток
        tasks.emplace_back(std::async (std::launch::async | std::launch::deferred, myLambda, std::move (item), currentDelay));
    }

    // Перебираю все задачи на предмет получения данных и завершения
    std::for_each(tasks.begin(),tasks.end(), [&]( std::future <std::string> & returnString){
        cout << returnString.get() << endl;
    } );

    end = std::chrono::high_resolution_clock::now();

    // Calculating total time taken by the program.
    time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    cout << "Time taken by program is : " << std::fixed << time_taken << '\n' <<
         "Total delay time: " << totalDelay << " mSec" << endl;

    std::cout << "End task 21.1 !" << std::endl;
    return 0;
}
