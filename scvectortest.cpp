#include "scvector.h"
int main()
{

   uint64_t items = 1000*1000*100;
   SCVector v(1024);
   time_t tsw = time(NULL);
   for (uint64_t i = 1; i < items; i++)
      v.push_back(i);
   cout << "write time = " << time(NULL) - tsw << endl;
   cout << v.print_stats();
   uint64_t x = 0;
   time_t tsr = time(NULL);
   cout << endl << "forward" << endl;
   for (SCVectorIterator begin_iter=v.begin(), end_iter=v.end(); begin_iter!=end_iter; ++begin_iter)
   {
      //cout << *begin_iter << endl;
      x++;
   }
   cout << "read time = " << time(NULL) - tsr << endl;
   cout << "Values read " << x << endl;
   cout << endl << "reverse" << endl;
   tsr = time(NULL); x = 0;
   for (SCVectorReverseIterator begin_iter=v.rbegin(), end_iter=v.rend(); begin_iter!=end_iter; ++begin_iter)
   {
      //cout << *begin_iter << endl;
      x++;
   }
   cout << "read time = " << time(NULL) - tsr << endl;
   cout << "Values read " << x << endl;

   return 0;
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

