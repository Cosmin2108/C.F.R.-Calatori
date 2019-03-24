#include <iostream>
#include <fstream>
#include<algorithm>
#include <stdlib.h>
using namespace std;

ifstream in("Date.txt");
ofstream out("Date_out.txt");

struct grup{
int sizee;
int statie_coborare;
int nr_compartiment;
int nr_vagon;
int nr_grup;
};

struct vagon{
  int compartiment[10]={8,8,8,8,8,8,8,8,8,8}; ///10 compartimente a cate 8 locuri(trebuie initializat)
};

int cmp(const void *i, const void *j)
{
    char a=((struct grup*)i)->sizee, b=((struct grup*)j)->sizee;
   if(a<b )
    return -1 ;
   else
    return 1;
}

void cautare_loc(vagon *tren,grup g, int &v, int &c)  /// cautam de preferat spatii libere mai <4
{  int k=0,last_sizee=8;
   v=-1;c=-1;
  while(k<5)   /// pe vagoane
  {
    for(int j=0;j<10;j++)   /// pe compartimente
    { if(tren[k].compartiment[j]==g.sizee)
      { v=k; c=j;
       return; }
      if(g.sizee<tren[k].compartiment[j] && tren[k].compartiment[j]<=last_sizee ) /// fac asta pt grupuri mai mici decat 4
      {  last_sizee=tren[k].compartiment[j];
         v=k; c=j; }
    }
    k++;
  }
}

void sparge(vagon *tren, int g, grup *v,int &n, int &nr_gr) /// nr_gr este numarul total de grupuri, g- poz grup actual in v
{                                     /// n este nr de grupuri din bucata de vector transmisa la distribuire locurilor
   int dim=v[g].sizee, liber=0,v1,c1,v2,c2,liber2=0, liber3=9;
   int k=0;
   while(k<5)
   {
    for(int i=0;i<10;i++)
    {
         if(tren[k].compartiment[i]>0&&liber<tren[k].compartiment[i])
           {
             liber=tren[k].compartiment[i];
             v1=k; c1=i;
           }
    }
    k++;
    }

    v[g].sizee=liber;
    v[g].nr_vagon=v1;
    v[g].nr_compartiment=c1;
    tren[v1].compartiment[c1]=0;


    k=0;
    while(k<5)
    {
     for(int i=0;i<10;i++)
     {
          if(tren[k].compartiment[i]+liber==dim)
            {v2=k; c2=i; return;}
          else
          if(tren[k].compartiment[i]+liber<dim&&liber2<tren[k].compartiment[i]+liber)
            {
              liber2=tren[k].compartiment[i]+liber;
              v2=k; c2=i;
            }
            else
            if(tren[k].compartiment[i]+liber>dim&&tren[k].compartiment[i]+liber<liber3)
            {
              liber3=tren[k].compartiment[i];
              v2=k; c2=i;

            }

     }
     k++;
     }

     if(liber3<9)
       {
           v[n].sizee=dim-liber;
           v[n].nr_grup=v[g].nr_grup;
           v[n].statie_coborare=v[g].statie_coborare;
           v[n].nr_compartiment=c2;
           v[n].nr_vagon=v2;
           n++;
           nr_gr++;
           tren[v2].compartiment[c2]-=(dim-liber);

       }
      else
      {
        v[n].sizee=liber2;
        v[n].nr_grup=v[g].nr_grup;
        v[n].statie_coborare=v[g].statie_coborare;
        v[n].nr_compartiment=c2;
        v[n].nr_vagon=v2;
        nr_gr++;
        n++;
        tren[v2].compartiment[c2]=0;

        v[n].sizee=dim-liber-liber2;
        v[n].nr_grup=v[g].nr_grup;
        v[n].statie_coborare=v[g].statie_coborare;
        nr_gr++;
        n++;
        cautare_loc(tren,v[n-1], v2,c2);

        if(v2==-1&&c2==-1)
           sparge(tren,n-1,v,n,nr_gr);
        else
        {
        tren[v2].compartiment[c2]-=v[n].sizee;
        v[n].nr_compartiment=c2;
        v[n].nr_vagon=v2;
        }

      }

}

void alocare_la_urcare(vagon *tren,grup *v,int n, int &nr_gr)
{  int vg,c, sparg=0, n2=n;
   cout<<"Nr grupuri "<<n<<"\n";
   int k=0;
  for(int i=0;i<n2;i++)  /// pe grupuri
    {
      int ok=0;

      while(k<5 && v[i].sizee>=4)   /// merg pe vagoane
      {
        for(int j=0;j<10 && !ok;j++)    /// pe compartimente
         if(tren[k].compartiment[j]>=5 ) /// fac asta pt grupuri mai mici decat 4
         {
         tren[k].compartiment[j]-=v[i].sizee;
         v[i].nr_compartiment=j;
         v[i].nr_vagon=k;
         ok=1;

         }
         if(ok)
         break;
         k++;
      }

      if(!ok)
       {
         cautare_loc(tren,v[i],vg,c);
         if(vg==-1&&c==-1)
           sparge(tren,i,v,n,nr_gr);

         else
         {
         tren[vg].compartiment[c]-=v[i].sizee;
         v[i].nr_compartiment=c;
         v[i].nr_vagon=vg;
         }
       }

    }

}

void coborare(vagon *tren, grup *v, int nr_grupuri, int s)
{
    int vag, comp, sizee, statie;
    for(int i=0;i<nr_grupuri;i++)
    {  statie=v[i].statie_coborare;
       if(s==statie)
        {
            vag= v[i].nr_vagon;
            comp=v[i].nr_compartiment;
            sizee=v[i].sizee;
           /// cout<<tren[vag].compartiment[comp]<<"Comp\n";
            tren[vag].compartiment[comp]+=sizee;
           /// cout<<tren[vag].compartiment[comp]<<"Comp\n";
        }
    }

}

int main()
{
int s,j,i,nr_gr=0,nr_grupuri_urca;
vagon *tren=new vagon[5];;
grup gr_v[420];
for(s=0;s<7;s++) ///statii
{
    coborare(tren, gr_v,nr_gr,s);
    in>>nr_grupuri_urca;

    for(i=0;i<nr_grupuri_urca;i++)
    {
      in>>gr_v[i+nr_gr].sizee>>gr_v[i+nr_gr].statie_coborare;
      gr_v[i+nr_gr].nr_grup=i+nr_gr;
    }

    qsort(gr_v+nr_gr,nr_grupuri_urca,sizeof(gr_v[0]),cmp);   ///sortam in ordine descrescatoare dupaa dim grup
    nr_gr=nr_gr+nr_grupuri_urca;
    alocare_la_urcare(tren,gr_v+(nr_gr-nr_grupuri_urca),nr_grupuri_urca, nr_gr);

}
   int sum=0;
   for(i=0;i<nr_gr;i++)
    {   cout<<i<<" Dim. grup: "<<gr_v[i].sizee<<" Vagon si compar. "<<gr_v[i].nr_vagon<<" || "<<gr_v[i].nr_compartiment<<endl ;
      ///  sum+=gr_v[i].sizee;
    }


return 0;
}
