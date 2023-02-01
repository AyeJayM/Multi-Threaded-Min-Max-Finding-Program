#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>

using namespace std;

constexpr long long value= 100;
unsigned long long minimum;
unsigned long long maximum;

mutex myMutex;

void sumUp(unsigned long long& minimum, unsigned long long& maximum, const vector<int>& val, 
   unsigned long long beg, unsigned long long end)
   {
   long long localMin = val[beg];
   long long localMax = val[beg];

    for (auto it= beg; it <= end; ++it)
    {

       if(val[it] > localMax)
       {
         localMax = val[it];
       }

       if(val[it] < localMin)
       {
         localMin = val[it];
       }
   
    }

    lock_guard<mutex> myLock(myMutex);

    if (localMin < minimum)
    {
      minimum = localMin;
    }

    if (maximum < localMax)
    {
      maximum = localMax;
    }

   }

int main()
{

  cout << endl;

  vector<int> randValues;
  randValues.reserve(value);

 // Random Number Generation
  mt19937 engine (0);
  uniform_int_distribution<> uniformDist(1,10000);
  for ( long long i=0 ; i< value ; ++i)
     randValues.push_back(uniformDist(engine));
 /////////////////////////////



// Print Generated Values
for(int cntr =0; cntr < value; cntr++)
{
  cout << "The " << cntr << " number generated" << ": " << randValues[cntr] << endl;
}
 minimum = randValues[0];
 maximum = randValues[0];
///////////////////////////
  


  auto start = chrono::system_clock::now();

  int threads = 4;
  thread t[threads];
  long long slice = value / threads;
  int startIdx=0;

  for (int i = 0; i < threads; ++i) 
  {
    cout << "Thread[" << i << "] - slice ["
         << startIdx << ":" << startIdx+slice-1 << "]" << endl;
    t[i] = thread(sumUp, ref(minimum), ref(maximum), ref(randValues), startIdx, startIdx+slice-1);
    startIdx += slice;
  }



  for (int i = 0; i < threads; ++i)
     t[i].join();

  chrono::duration<double> dur= chrono::system_clock::now() - start;
  cout << "Time to find min / max " << dur.count() << " seconds" << endl;
  cout << "\nMinimum: " << minimum << endl;
  cout << "\nMaximum: " << maximum << endl;

  cout << endl;

}