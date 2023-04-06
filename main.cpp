#include <iostream>
#include <string>  
#include <stdexcept>  
#include <stdlib.h>
  
using namespace std;  
  
class Move {  
private:  
  int c1,c2;  
  int h1,h2;  
  
public:  
  
  Move(int sh, int sc, int th, int tc)  
  {  
    c1=sc; h1=sh; c2=tc; h2=th;  
  }  
  int getSource() const  
  {  
    return h1;  
  }  
  int getSourceCoins() const  
  {  
    return c1;  
  }  
  int getTarget() const  
  {  
    return h2;  
  }  
  int getTargetCoins() const  
  {  
    return c2;  
  }  
  friend ostream & operator << (ostream &out, const Move &move)  
  {  
    if (move.getTargetCoins()==0)  
    {  
        out << "takes "<< move.getSourceCoins()
            <<" coins from heap "<<move.getSource()
            << " and puts nothing";  
  
        return out;  
    }  
    out << "takes "<< move.getSourceCoins()
        <<" coins from heap " <<move.getSource()
        << " and puts "<<move.getTargetCoins()
        <<" coins to heap "<<move.getTarget();  
  
    return out ;  
  }  
};  


class State  
{  
private:  
  int *arr;  
  int heaps;  
  int all_heaps;  
  
public:  
  State(int h, const int c[])  
  {  
    heaps=h;  
    all_heaps=h;  
    arr = new int[heaps];  
    for (int i=0; i<heaps; i++)  
      arr[i]=c[i];  
  }  
  ~State()  
  {  
    delete[] arr;  
  }    
  void next(const Move &move) noexcept(false)
  {  
    if ((move.getSourceCoins()<=0 ||  move.getTargetCoins()<0) ||               
        (move.getSourceCoins()<=move.getTargetCoins()) || 
        (move.getSource()<0)||  
        (move.getTarget()<0)|| 
        (arr[move.getSource()]<move.getSourceCoins())|| 
        (move.getSource() > all_heaps)|| 
        (move.getTarget() > all_heaps))  
      
      throw logic_error("void next");  
  
    arr[move.getSource()]= arr[move.getSource()] - move.getSourceCoins();  
    arr[move.getTarget()]= arr[move.getTarget()] + move.getTargetCoins();  
  
    if (arr[move.getSource()]==0)  
      heaps--;  
  
  } 

  bool winning() const  
  {  
    int i=0;  
    bool k=0;  
    for (i=0; i<all_heaps; i++)  
    {  
        if (arr[i] != 0)  
        {  
            k=0;  
            break;  
        }  
        k=1;  
    }  
  
    return k;  
  }  
  
  int getHeaps() const  
  {  
    return heaps;  
  }  
  int getCoins(int h)  const noexcept(false)   
  {  
    if (h >= all_heaps || h<0)  
      throw logic_error ("get coins");  
  
    return arr[h];  
  }  
  friend ostream & operator << (ostream &out, const State &state)  
  {  
    if (state.all_heaps == 1)  
    {  
      out << state.arr[0];  
      return out;  
    }  
    if (state.all_heaps == 2)  
    {  
      out << state.arr[0]<<", "<<state.arr[1];  
      return out;  
    }  
    if (state.all_heaps==3)  
    {  
        out <<state.arr[0]<<", "<<state.arr[1]<<", "<<state.arr[2];  
        return out;  
    }  
    for (int i=0; i<state.all_heaps; i++)  
    {  
      if (i== state.heaps-1)  
      {  
        out <<state.arr[state.heaps-1];  
        return out;  
      }  
      out << state.arr[i]<<", ";  
  
    }  
  }  
};

class Player  
{  
protected:  
string onoma;  
  
public:  
  Player (const string &n)  
  {  
    onoma = n;  
  }  
  virtual ~Player()  
  {  
  
  }  
  virtual const string & getType() const = 0;  
  virtual Move play(const State &s) = 0;  
  
  friend ostream & operator << (ostream &out, const Player &player)    
  {  
    out << player.getType() << " player "<< player.onoma;  
    return out;  
  }  
};  
class GreedyPlayer :public Player  
{  
private:  
  
public:  
  GreedyPlayer(const string &n) :Player(n)  {}  
  
  const string & getType() const override  
  {  
    static string type = "Greedy";  
    return type;  
  }  
  
  Move play(const State &s)  override
  {  
    int max=0,max_h;  
    for (int i=0; i<s.getHeaps(); i++)  
    {  
      if (s.getCoins(i) > max)  
      {  
        max=s.getCoins(i);  
        max_h=i;  
      }  
    }  
    return Move (max_h,s.getCoins(max_h),0,0);  
  }  
};  

class SpartanPlayer : public Player  
{  
public:  
  SpartanPlayer (const string &n) :Player(n){}  
  const string & getType() const override  
  {  
    static string type = "Spartan";  
    return type;  
  }  
  Move play(const State &s) override
  {  
    int max=0,max_h;  
    for (int i=0; i<s.getHeaps(); i++)  
    {  
      if (s.getCoins(i) > max)  
      {  
        max=s.getCoins(i);  
        max_h=i;  
      }  
    }  
    return Move (max_h,1,0,0);  
  }  
};  
class SneakyPlayer :public Player  
{  
  
public:  
  
  SneakyPlayer (const string &n): Player (n){}  
  const string & getType() const override  
  {  
    static string type = "Sneaky";  
    return type;  
  }  
  Move play (const State &s) override 
  {  
    int min=100000,min_h;  
    for (int i=0; i<s.getHeaps(); i++)  
    {  
      if (min > s.getCoins(i) &&(s.getCoins(i) != 0))  
      {  
        min = s.getCoins(i);  
        min_h=i;  
      }  
    }  
    return Move(min_h,s.getCoins(min_h),0,0);  
  }  
};  
class RighteousPlayer :public Player  
{  
public:  
  RighteousPlayer (const string &n): Player (n){}  
  const string & getType() const override  
  {  
    static string type="Righteous";  
    return type;  
  }  
  Move play (const State &s) override 
  {  
    int max=0,max_h;  
    for (int i=0; i<s.getHeaps(); i++)  
    {  
      if (s.getCoins(i) > max)  
      {  
        max=s.getCoins(i);  
        max_h=i;  
      }  
    }  
  
    int min=100000,min_h;  
    for (int i=0; i<s.getHeaps(); i++)  
    {  
      if (s.getCoins(i) < min )  
      {  
        min = s.getCoins(i);  
        min_h=i;  
      }  
    }  
    if (s.getCoins(max_h)%2==0)  
    {  
        return Move(max_h , s.getCoins(max_h)/2 , min_h , ((s.getCoins(max_h))/2- 1) );  
    }  
    else  
        return Move(max_h , s.getCoins(max_h)/2+1 , min_h , ((s.getCoins(max_h))/2) );  
  
  }  
}; 
class Game 
{
private:
int h;
int p;
int counter_of_p;
int *c; // arr of coins
Player **pl; // arr of players types
int j;

public:
  Game(int heaps, int players)
  {
    h=heaps;
    p=players;
    j=0;
    counter_of_p=0;
    c = new int [h];
    pl = (Player**)malloc(p*sizeof(Player*));
    
  }
  ~Game()
  {

  }
  void addHeap(int coins) noexcept(false) 
  {
    if (coins < 0 ) logic_error ("invalid heap");
    
    c[j++]=coins;

  }
  void addPlayer(Player *player) noexcept(false)
  {
    if (counter_of_p > p)
      logic_error("invalid");

      
      pl[counter_of_p++]= player; //  array with Players

  }
  void play(ostream &out) noexcept(false)
  { 
    State s(h,c);
    int i=0;
  
    cout << *pl[0];
  }
};
int main ()
{ 
 Game specker(1, 1);
 specker.addHeap(1);
 specker.addPlayer(new RighteousPlayer("Robin"));
 
 specker.play(cout);
  
}
