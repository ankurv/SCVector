#include "scvector.h"
int main()
{
   uint64_t items = 1000*1000*100;
   SCVector v;
   time_t tsw = time(NULL);
   for (uint64_t i = 1; i < items; i++)
      v.push_back(i);
   cout << "write time = " << time(NULL) - tsw << endl;
   uint64_t x = 0;
   time_t tsr = time(NULL);
   for (SCVectorIterator begin_iter=v.begin(), end_iter=v.end(); begin_iter!=end_iter; ++begin_iter)
   {
      //cout << *begin_iter<< ", ";
      x++;
   }
   cout << "read time = " << time(NULL) - tsr << endl;
   cout << "Values read " << x << endl;
   cout << v.print_stats();

   vector<uint64_t> vv;
   time_t tvw = time(NULL);
   for (uint64_t i = 1; i < items ; i++)
      vv.push_back(i);
   x = 0;
   cout << "write time = " << time(NULL) - tvw << endl;
   time_t tvr = time(NULL);
   for (vector<uint64_t>::iterator begin_iter=vv.begin(), end_iter=vv.end(); begin_iter!=end_iter; ++begin_iter)
   {
      //cout << *begin_iter<< ", ";
      x++;
   }
   cout << "read time = " << time(NULL) - tvr << endl;
   cout << "Values read vector " << x << endl;
   return 0;
}

