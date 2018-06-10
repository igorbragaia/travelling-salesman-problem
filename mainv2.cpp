#include<bits/stdc++.h>
using namespace std;
typedef pair<float,float> pp;
#define INF 2<<29


struct HeapElement{
  int weight, min_parent, index;
  HeapElement(int wweight, int mmin_parent, int iindex): weight(wweight), min_parent(mmin_parent), index(iindex) {};
  bool operator<(const HeapElement & x) const {
    if(weight < x.weight)
      return true;
    if(weight > x.weight)
        return false;
    if(min_parent < x.min_parent)
      return true;
    if(min_parent > x.min_parent)
      return false;
    return index < x.index;
  }
};


class Heap{
public:
  Heap(int n){
      size = 0;
      heap.resize(n, HeapElement(-1,-1,-1));
      positions.resize(n,-1);
  };

  HeapElement push(int weight, int min_parent, int index){
    HeapElement el(weight, min_parent, index);
    modify(++size, el);
  };

  HeapElement extractMin(){
    if(size == 1){
      HeapElement top = heap[1];
      size--;
      return top;
    } else if(size > 1){
      HeapElement top = heap[1];
      modify(1, heap[size--]);
      return top;
    } else {
      throw invalid_argument("Heap underflow");
    }
  };

  void modify(int k, HeapElement el){
    if(k < 1 or k > size){
      throw invalid_argument("Index error");
    } else {
      heap[k] = el;
      positions[el.index] = k;
      while(k > 1 and heap[k] < heap[k/2]){
        HeapElement aux = heap[k];
        heap[k] = heap[k/2];
        heap[k/2] = aux;
        k = k/2;

        positions[heap[k].index] = k;
        positions[heap[k/2].index] = k/2;
      }
      sift(k);
    }
  }

  int getSize(){
    return size;
  }
private:
  int size;
  vector<HeapElement>heap;
  vector<int>positions;

  void sift(int k){
    int esq = 2*k, dir = 2*k+1, menor = k;
    if(esq <= size and heap[esq] < heap[menor])
      menor = esq;
    if(dir <= size and heap[dir] < heap[menor])
      menor = dir;
    if(menor != k){
      HeapElement aux = heap[k];
      heap[k] = heap[menor];
      heap[menor] = aux;

      positions[heap[k].index] = k;
      positions[heap[menor].index] = menor;

      sift(menor);
    }
  };
};


































class Solution{
public:
    Solution(vector<pp>&vs, unsigned int&v){
        total_vertices = v;
        vertices = vs;
        int start = 1;
        create_mst(start);
        create_cycle(start);
    }

private:
    static int dij(pp i, pp j){
        int xd = i.first - j.first, yd = i.second - j.second;
        float x = sqrt(xd*xd + yd*yd);
        return (int)(x + 0.5);
    }
};


int main(){
    int n=1;
//    cin >> n;

    ofstream writingfile;
    writingfile.open("saida.txt");

    /* vertices variables */
    unsigned int total_vertices;
    int vertice, x, y;
    vector<pp> vertices;
    /* vertices variables */

    for(int i = 1; i <= n; i++){
        string filename;
        filename = "ent01.txt";//"ent"  + (i<10)?"0":"" + to_string(i) + ".txt";

        ifstream readingfile(filename);

        readingfile >> total_vertices;
        vertices.clear();
        vertices.resize(total_vertices + 1);

        for(unsigned int j = 1; j <= total_vertices; j++){
            readingfile >> vertice >> x >> y;
            vertices[vertice] = make_pair(x,y);
        }
        
        readingfile.close();

        Solution * solve = new Solution(vertices,total_vertices);
        solve->print_cycle();
        delete solve;
    }
    writingfile.close();

    return 0;
}
