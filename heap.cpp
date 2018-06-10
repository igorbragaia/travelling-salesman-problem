  #include<bits/stdc++.h>
  using namespace std;
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
        heap.resize(n + 1, HeapElement(-1,-1,-1));
        positions.resize(n + 1,-1);
    };

    HeapElement push(int weight, int min_parent, int index){
      HeapElement el(weight, min_parent, index);
      modify(++size, el);
    };

    HeapElement extractMin(){
      if(size < 1){
        throw invalid_argument("Heap underflow");
      } else if(size == 1){
        HeapElement top = heap[1];
        positions[top.index] = -1;
        size--;
        return top;
      } else if(size > 1){
        HeapElement top = heap[1];
        positions[top.index] = -1;
        modify(1, heap[size--]);
        return top;
      }
    };

    void modify(int k, HeapElement el){
      if(k < 1 or k > size){
        throw invalid_argument("Index error");
      } else {
        heap[k] = el;
        positions[heap[k].index] = k;

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

    bool hasVertex(int k){
      return positions[k] != -1;
    }

    int getVertex(int k){
      return positions[k];
    }

    HeapElement heapVertex(int k){
      return heap[positions[k]];
    }

    bool empty(){
      return !(size > 0);
    }
    vector<int>positions;
  private:
    int size;
    vector<HeapElement>heap;

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


  int main(){
      Heap heap(20);

      heap.push(0, 1, 1);
      heap.push(2, 1, 2);
      heap.push(3, 1, 3);
      heap.push(2, 1, 4);
      heap.push(3, 1, 5);
      heap.push(3, 1, 6);
      heap.push(4, 1, 7);
      heap.push(4, 1, 8);

      for(int i = 1; i <= 20; i++){
        printf("%d ", heap.positions[i]);
      }


      while(!heap.empty()){
        HeapElement top = heap.extractMin();
        printf("%d, %d, %d\n", top.weight, top.min_parent, top.index);
      }
      return 0;
  }
