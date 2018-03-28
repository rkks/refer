
        This file contains the code from "Algorithms in C, Third Edition,
        Part 5," by Robert Sedgewick, and is covered under the copyright
        and warranty notices in that book. Permission is granted for this
        code to be used for educational purposes in association with the text,
        and for other uses not covered by copyright laws, provided that
        the following notice is included with the code:

                "This code is from "Algorithms in C, Third Edition,"
                by Robert Sedgewick, Addison-Wesley, 2002."

        Commercial uses of this code require the explicit written
        permission of the publisher. Send your request for permission,
        stating clearly what code you would like to use, and in what
        specific way, to: aw.cse@aw.com


----------
CHAPTER 17. Graph Properties and Types
-----
typedef struct { int v; int w; } Edge;
Edge EDGE(int, int);

typedef struct graph *Graph;
Graph GRAPHinit(int);
 void GRAPHinsertE(Graph, Edge);
 void GRAPHremoveE(Graph, Edge);
  int GRAPHedges(Edge [], Graph G);
Graph GRAPHcopy(Graph);
 void GRAPHdestroy(Graph);
-----
#include <stdio.h>
#include "GRAPH.h"
main(int argc, char *argv[])
  { int V = atoi(argv[1]), E = atoi(argv[2]);
    Graph G = GRAPHrand(V, E);
    if (V < 20) 
         GRAPHshow(G);
    else printf("%d vertices, %d edges, ", V, E);
    printf("%d component(s)\n", GRAPHcc(G));
  }
-----
#include <stdlib.h>
#include "GRAPH.h"
struct graph { int V; int E; int **adj; };
Graph GRAPHinit(int V)
  { Graph G = malloc(sizeof *G);
    G->V = V; G->E = 0;
    G->adj = MATRIXint(V, V, 0);
    return G;
  }
void GRAPHinsertE(Graph G, Edge e)
  { int v = e.v, w = e.w;
    if (G->adj[v][w] == 0) G->E++;
    G->adj[v][w] = 1; 
    G->adj[w][v] = 1;
  }
void GRAPHremoveE(Graph G, Edge e)
  { int v = e.v, w = e.w;
    if (G->adj[v][w] == 1) G->E--;
    G->adj[v][w] = 0; 
    G->adj[w][v] = 0;
  }
int GRAPHedges(Edge a[], Graph G)
  { int v, w, E = 0;
    for (v = 0; v < G->V; v++)
      for (w = v+1; w < G->V; w++)
        if (G->adj[v][w] == 1) 
          a[E++] = EDGE(v, w); 
    return E;
  }
-----
int **MATRIXint(int r, int c, int val)
  { int i, j;
    int **t = malloc(r * sizeof(int *));
    for (i = 0; i < r; i++)
      t[i] = malloc(c * sizeof(int));
    for (i = 0; i < r; i++)
      for (j = 0; j < c; j++)
        t[i][j] = val;
    return t;
  }
-----
void GRAPHshow(Graph G)
  { int i, j; 
    printf("%d vertices, %d edges\n", G->V, G->E);
    for (i = 0; i < G->V; i++)
      {
        printf("%2d:", i);
        for (j = 0; j < G->V; j++)
          if (G->adj[i][j] == 1) printf(" %2d", j);
        printf("\n");
      }
  }
-----
#include <stdlib.h>
#include "GRAPH.h"
typedef struct node *link;
struct node { int v; link next; };
struct graph { int V; int E; link *adj; };
link NEW(int v, link next)
  { link x = malloc(sizeof *x);
    x->v = v; x->next = next;     
    return x;                         
  }
Graph GRAPHinit(int V)
  { int v;
    Graph G = malloc(sizeof *G);
    G->V = V; G->E = 0;
    G->adj = malloc(V*sizeof(link));
    for (v = 0; v < V; v++) G->adj[v] = NULL;
    return G;
  }
void GRAPHinsertE(Graph G, Edge e)
  { int v = e.v, w = e.w;
    G->adj[v] = NEW(w, G->adj[v]);
    G->adj[w] = NEW(v, G->adj[w]); 
    G->E++;
  }
int GRAPHedges(Edge a[], Graph G)
  { int v, E = 0; link t;  
    for (v = 0; v < G->V; v++)
      for (t = G->adj[v]; t != NULL; t = t->next)
        if (v < t->v) a[E++] = EDGE(v, t->v); 
    return E;
  }
-----
int randV(Graph G)
  { return G->V * (rand() / (RAND_MAX + 1.0)); }
Graph GRAPHrand(int V, int E)
  { Graph G = GRAPHinit(V);
    while (G->E < E)
      GRAPHinsertE(G, EDGE(randV(G), randV(G)));
    return G;
  }
-----
Graph GRAPHrand(int V, int E)
  { int i, j;
    double p = 2.0*E/V/(V-1);
    Graph G = GRAPHinit(V);
    for (i = 0; i < V; i++)
      for (j = 0; j < i; j++)
        if (rand() < p*RAND_MAX)
          GRAPHinsertE(G, EDGE(i, j));
    return G;
  }
-----
#include <stdio.h>
#include "GRAPH.h"
#include "ST.h"
Graph GRAPHscan(int Vmax, int Emax)
  { char v[100], w[100];
    Graph G = GRAPHinit(Vmax);
    STinit();
    while (scanf("%99s %99s", v, w) == 2)
      GRAPHinsertE(G, EDGE(STindex(v), STindex(w)));
    return G;
  }
-----
#include <stdlib.h>
typedef struct STnode* link;
struct STnode { int index, d; link l, m, r; };
static link head;
static int val, N; 
void STinit() 
  { head = NULL; N = 0; }
link stNEW(int d)
  { link x = malloc(sizeof *x);   
    x->index = -1; x->d = d; 
    x->l = NULL; x->m = NULL; x->r = NULL;
    return x;
  }
link indexR(link h, char* v, int w)
  { int i = v[w];
    if (h == NULL) h = stNEW(i); 
    if (i == 0) 
      { 
        if (h->index == -1) h->index = N++;
        val = h->index;
        return h;
      }
    if (i < h->d) h->l = indexR(h->l, v, w);
    if (i == h->d) h->m = indexR(h->m, v, w+1);
    if (i > h->d) h->r = indexR(h->r, v, w);
    return h;
  }
int STindex(char* key)
  { head = indexR(head, key, 0); return val; }
-----
static int visited[maxV];
int pathR(Graph G, int v, int w)
  { int t; 
    if (v == w) return 1;
    visited[v] = 1;
    for (t = 0; t < G->V; t++)
      if (G->adj[v][t] == 1)
        if (visited[t] == 0)
          if (pathR(G, t, w)) return 1;
    return 0;
  }
int GRAPHpath(Graph G, int v, int w)
  { int t; 
    for (t = 0; t < G->V; t++) visited[t] = 0;
    return pathR(G, v, w);
  }
-----
static int visited[maxV];
int pathR(Graph G, int v, int w, int d)
  { int t; 
    if (v == w) 
      { if (d == 0) return 1; else return 0; }
    visited[v] = 1; 
    for (t = 0; t < G->V; t++)
      if (G->adj[v][t] == 1)
        if (visited[t] == 0)
          if (pathR(G, t, w, d-1)) return 1;
    visited[v] = 0;
    return 0;
  }
int GRAPHpathH(Graph G, int v, int w)
  { int t; 
    for (t = 0; t < G->V; t++) visited[t] = 0;
    return pathR(G, v, w, G->V-1);
  }
-----
int GRAPHpathE(Graph G, int v, int w)
  { int t;
    t = GRAPHdeg(G, v) + GRAPHdeg(G, w);
    if ((t % 2) != 0) return 0;
    for (t = 0; t < G->V; t++)
      if ((t != v) && (t != w))
        if ((GRAPHdeg(G, t) % 2) != 0) return 0;
    return 1;
  }
-----
#include "STACK.h"
int path(Graph G, int v)
  { int w; 
    for (; G->adj[v] != NULL; v = w)
      { 
        STACKpush(v); 
        w = G->adj[v]->v; 
        GRAPHremoveE(G, EDGE(v, w)); 
      }
    return v;
  }
void pathEshow(Graph G, int v, int w)
  { 
    STACKinit(G->E);
    printf("%d", w);
    while ((path(G, v) == v) && !STACKempty())
      { v = STACKpop(); printf("-%d", v); }
    printf("\n");
  }
-----
#define GRAPHiso(G, v) (GRAPHdeg(G, v) == 0)
void pathEshow(Graph G, int v, int w)
  { int t; 
    if ((v == w) && (G->E == 0)) return;
    for (t = 0; t < G->V; t++)
      if (G->adj[v][t] != 0) 
        {
          GRAPHremoveE(G, EDGE(v, t));
          if (GRAPHiso(G, v) || GRAPHpath(G, t, v))
            {
              printf("%d-%d\n", v, t); 
              pathEshow(G, t, w);
              GRAPHinsertE(G, EDGE(v, t));
              return; 
            }
          GRAPHinsertE(G, EDGE(v, t));
        }
  }

----------
CHAPTER 18. Graph Search
-----
#define dfsR search
void dfsR(Graph G, Edge e)
  { int t, w = e.w;
    pre[w] = cnt++; 
    for (t = 0; t < G->V; t++)
      if (G->adj[w][t] != 0) 
        if (pre[t] == -1)
          dfsR(G, EDGE(w, t)); 
  }
-----
void dfsR(Graph G, Edge e)
  { link t; int w = e.w;
    pre[w] = cnt++; 
    for (t = G->adj[w]; t != NULL; t = t->next)
      if (pre[t->v] == -1) 
        dfsR(G, EDGE(w, t->v)); 
  }
-----
static int cnt, pre[maxV];
void GRAPHsearch(Graph G)
  { int v;
    cnt = 0;
    for (v = 0; v < G->V; v++) pre[v] = -1;
    for (v = 0; v < G->V; v++)
      if (pre[v] == -1) 
        search(G, EDGE(v, v));
  }
-----
void dfsRcc(Graph G, int v, int id)
  { link t; 
    G->cc[v] = id;
    for (t = G->adj[v]; t != NULL; t = t->next)
      if (G->cc[t->v] == -1) dfsRcc(G, t->v, id); 
  }
int GRAPHcc(Graph G)
  { int v, id = 0;
    G->cc = malloc(G->V * sizeof(int));
    for (v = 0; v < G->V; v++) 
      G->cc[v] = -1;
    for (v = 0; v < G->V; v++)
      if (G->cc[v] == -1) dfsRcc(G, v, id++);
    return id;
  }
int GRAPHconnect(Graph G, int s, int t)
  { return G->cc[s] == G->cc[t]; }
-----
void dfsReuler(Graph G, Edge e)
  { link t;
    printf("-%d", e.w);
    pre[e.w] = cnt++;
    for (t = G->adj[e.w]; t != NULL; t = t->next)
      if (pre[t->v] == -1) 
        dfsReuler(G, EDGE(e.w, t->v)); 
      else if (pre[t->v] < pre[e.v])
        printf("-%d-%d", t->v, e.w);
    if (e.v != e.w) 
         printf("-%d", e.v);
    else printf("\n");
  }
-----
int dfsRcolor(Graph G, int v, int c)
  { link t; 
    G->color[v] = 1-c;
    for (t = G->adj[v]; t != NULL; t = t->next)
      if (G->color[t->v] == -1) 
        { if (!dfsRcolor(G, t->v, 1-c)) return 0; }
      else if (G->color[t->v] != c) return 0;
    return 1;
  }
int GRAPHtwocolor(Graph G)
  { int v, id = 0;
    G->color = malloc(G->V * sizeof(int));
    for (v = 0; v < G->V; v++) 
      G->color[v] = -1;
    for (v = 0; v < G->V; v++)
      if (G->color[v] == -1) 
        if (!dfsRcolor(G, v, 0)) return 0;
    return 1;
  }
-----
void bridgeR(Graph G, Edge e)
  { link t; int v, w = e.w;
    pre[w] = cnt++; low[w] = pre[w];
    for (t = G->adj[w]; t != NULL; t = t->next)
      if (pre[v = t->v] == -1) 
        {
          bridgeR(G, EDGE(w, v)); 
          if (low[w] > low[v]) low[w] = low[v];
          if (low[v] == pre[v]) 
            bcnt++; printf("%d-%d\n", w, v);
        }
      else if (v != e.v)
        if (low[w] > pre[v]) low[w] = pre[v];
  }
-----
#define bfs search
void bfs(Graph G, Edge e)
  { int v, w;
    QUEUEput(e);
    while (!QUEUEempty())
      if (pre[(e = QUEUEget()).w] == -1)
        {
          pre[e.w] = cnt++; st[e.w] = e.v;
          for (v = 0; v < G->V; v++)
            if (G->adj[e.w][v] == 1)
              if (pre[v] == -1)
                QUEUEput(EDGE(e.w, v)); 
        }
  }
-----
void bfs(Graph G, Edge e)
  { int v, w;
    QUEUEput(e); pre[e.w] = cnt++; 
    while (!QUEUEempty())
      {
        e = QUEUEget(); 
        w = e.w; st[w] = e.v; 
        for (v = 0; v < G->V; v++)
          if ((G->adj[w][v] == 1) && (pre[v] == -1))
           { QUEUEput(EDGE(w, v)); pre[v] = cnt++; }
      }
  }
-----
#define pfs search
void pfs(Graph G, Edge e)
  { link t; int v, w;
    GQput(e); pre[e.w] = cnt++;
    while (!GQempty())
      {
        e = GQget(); w = e.w; st[w] = e.v; 
        for (t = G->adj[w]; t != NULL; t = t->next)
          if (pre[v = t->v] == -1)
            { GQput(EDGE(w, v)); pre[v] = cnt++; }
          else if (st[v] == -1)
            GQupdate(EDGE(w, v));
      }
  }
-----
#include <stdlib.h>
#include "GQ.h"
static Item *s;
static int N;
void RQinit(int maxN)
  { s = malloc(maxN*sizeof(Item)); N = 0; }
int RQempty()
  { return N == 0; }
void RQput(Item x)
  { s[N++] = x; }
void RQupdate(Item x)
  { }
Item RQget()
  { Item t;
    int i = N*(rand()/(RAND_MAX + 1.0));
    t = s[i]; s[i] = s[N-1]; s[N-1] = t;    
    return s[--N]; 
  }

----------
CHAPTER 19. Digraphs and DAGs
-----
Graph GRAPHreverse(Graph G)
  { int v; link t;
    Graph R = GRAPHinit(G->V);
    for (v = 0; v < G->V; v++) 
      for (t = G->adj[v]; t != NULL; t = t->next)
        GRAPHinsertE(R, EDGE(t->v, v));        
    return R;
  }
-----
void dfsR(Graph G, Edge e)
  { link t; int i, v, w = e.w; Edge x;
    show("tree", e);
    pre[w] = cnt++; 
    for (t = G->adj[w]; t != NULL; t = t->next)
      if (pre[t->v] == -1) 
        dfsR(G, EDGE(w, t->v)); 
      else 
        { v = t->v; x = EDGE(w, v);
          if (post[v] == -1) show("back", x);
          else if (pre[v] > pre[w]) show("down", x);
          else show("cross", x);
        }
    post[w] = cntP++; 
  }
-----
void GRAPHtc(Graph G)
  { int i, s, t;
    G->tc = MATRIXint(G->V, G->V, 0);
    for (s = 0; s < G->V; s++)
      for (t = 0; t < G->V; t++)
        G->tc[s][t] = G->adj[s][t];
    for (s = 0; s < G->V; s++) G->tc[s][s] = 1;
    for (i = 0; i < G->V; i++)
      for (s = 0; s < G->V; s++)
        if (G->tc[s][i] == 1)
          for (t = 0; t < G->V; t++)
            if (G->tc[i][t] == 1) G->tc[s][t] = 1; 
  }
int GRAPHreach(Graph G, int s, int t)
  { return G->tc[s][t]; }
-----
void TCdfsR(Graph G, Edge e)
  { link t; 
    G->tc[e.v][e.w] = 1; 
    for (t = G->adj[e.w]; t != NULL; t = t->next)
      if (G->tc[e.v][t->v] == 0) 
        TCdfsR(G, EDGE(e.v, t->v)); 
  }
void GRAPHtc(Graph G, Edge e)
  { int v, w;
    G->tc = MATRIXint(G->V, G->V, 0);
    for (v = 0; v < G->V; v++) 
      TCdfsR(G, EDGE(v, v));
  }
int GRAPHreach(Graph G, int s, int t)
  { return G->tc[s][t]; }
-----
int compressR(link h)
  { int l, r, t; 
    if (h == NULL) return 0;
    l = compressR(h->l); 
    r = compressR(h->r);
    t = STindex(l*Vmax + r);
    adj[t].l = l; adj[t].r = r;
    return t;
  }
-----
static int cnt0;
static int pre[maxV];
void DAGts(Dag D, int ts[])
  { int v; 
    cnt0 = 0;
    for (v = 0; v < D->V; v++) 
      { ts[v] = -1; pre[v] = -1; }
    for (v = 0; v < D->V; v++)
      if (pre[v] == -1) TSdfsR(D, v, ts);
}
void TSdfsR(Dag D, int v, int ts[])
  { link t; 
    pre[v] = 0; 
    for (t = D->adj[v]; t != NULL; t = t->next)
      if (pre[t->v] == -1) TSdfsR(D, t->v, ts); 
    ts[cnt0++] = v;
  }
-----
void TSdfsR(Dag D, int v, int ts[])
  { int w;
    pre[v] = 0; 
    for (w = 0; w < D->V; w++)
      if (D->adj[w][v] != 0) 
        if (pre[w] == -1) TSdfsR(D, w, ts); 
    ts[cnt0++] = v;
  }
-----
#include "QUEUE.h"
static int in[maxV];
void DAGts(Dag D, int ts[])
  { int i, v; link t;
    for (v = 0; v < D->V; v++) 
      { in[v] = 0; ts[v] = -1; }
    for (v = 0; v < D->V; v++) 
      for (t = D->adj[v]; t != NULL; t = t->next)
        in[t->v]++;
    QUEUEinit(D->V);
    for (v = 0; v < D->V; v++)
      if (in[v] == 0) QUEUEput(v);
    for (i = 0; !QUEUEempty(); i++)
      {
        ts[i] = (v = QUEUEget());
        for (t = D->adj[v]; t != NULL; t = t->next)
          if (--in[t->v] == 0) QUEUEput(t->v);
      }
  }
-----
void DAGtc(Dag D)
  { int v;
    D->tc = MATRIXint(D->V, D->V, 0);
    for (v = 0; v < D->V; v++) pre[v] = -1; 
    for (v = 0; v < D->V; v++) 
      if (pre[v] == -1) TCdfsR(D, EDGE(v, v)); 
  }
void TCdfsR(Dag D, Edge e)
  { int u, i, v = e.w;
    pre[v] = cnt++;
    for (u = 0; u < D->V; u++)
      if (D->adj[v][u] != 0)
        { 
          D->tc[v][u] = 1; 
          if (pre[u] > pre[v]) continue;
          if (pre[u] == -1) TCdfsR(D, EDGE(v, u)); 
          for (i = 0; i < D->V; i++)
            if (D->tc[u][i] == 1) D->tc[v][i] = 1;
        }            
  }
int DAGreach(Dag D, int s, int t)
  { return D->tc[s][t]; }
-----
static int post[maxV], postR[maxV];
static int cnt0, cnt1;
void SCdfsR(Graph G, int w)
  { link t;
    G->sc[w] = cnt1; 
    for (t = G->adj[w]; t != NULL; t = t->next)
      if (G->sc[t->v] == -1) SCdfsR(G, t->v); 
    post[cnt0++] = w;
  }
int GRAPHsc(Graph G)
  { int v; Graph R;
    R = GRAPHreverse(G);
    cnt0 = 0; cnt1 = 0;
    for (v = 0; v < G->V; v++) R->sc[v] = -1;
    for (v = 0; v < G->V; v++)
      if (R->sc[v] == -1) SCdfsR(R, v);
    cnt0 = 0; cnt1 = 0;
    for (v = 0; v < G->V; v++) G->sc[v] = -1;
    for (v = 0; v < G->V; v++) postR[v] = post[v];
    for (v = G->V-1; v >=0; v--)
      if (G->sc[postR[v]] == -1) 
        { SCdfsR(G, postR[v]); cnt1++; }
    GRAPHdestroy(R);
    return cnt1;
  }
int GRAPHstrongreach(Graph G, int s, int t)
  { return G->sc[s] == G->sc[t]; }
-----
void SCdfsR(Graph G, int w)
  { link t; int v, min;
    pre[w] = cnt0++; low[w] = pre[w]; min = low[w];
    s[N++] = w; 
    for (t = G->adj[w]; t != NULL; t = t->next)
      {
        if (pre[t->v] == -1) SCdfsR(G, t->v);
        if (low[t->v] < min) min = low[t->v];
      }
    if (min < low[w]) { low[w] = min; return; }
    do
      { G->sc[(v = s[--N])] = cnt1; low[v] = G->V; }
    while (s[N] != w); 
    cnt1++;
  }
-----
void SCdfsR(Graph G, int w)
  { link t; int v;
    pre[w] = cnt0++; 
    s[N++] = w; path[p++] = w; 
    for (t = G->adj[w]; t != NULL; t = t->next)
      if (pre[t->v] == -1) SCdfsR(G, t->v);
      else if (G->sc[t->v] == -1) 
             while (pre[path[p-1]] > pre[t->v]) p--;
    if (path[p-1] != w) return; else p--;
    do G->sc[s[--N]] = cnt1; while (s[N] != w);
    cnt1++;
  }
-----
Dag K;
void GRAPHtc(Graph G)
  { int v, w; link t; int *sc = G->sc;
    K = DAGinit(GRAPHsc(G));
    for (v = 0; v < G->V; v++) 
      for (t = G->adj[v]; t != NULL; t = t->next)
        DAGinsertE(K, dagEDGE(sc[v], sc[t->v]));
    DAGtc(K);
  }
int GRAPHreach(Graph G, int s, int t)
  { return DAGreach(K, G->sc[s], G->sc[t]); }

----------
CHAPTER 20. Minimum Spanning Trees
-----
#include <stdlib.h>
#include "GRAPH.h"
struct graph { int V; int E; double **adj; };
Graph GRAPHinit(int V)
  { int v;
    Graph G = malloc(sizeof *G);
    G->adj = MATRIXdouble(V, V, maxWT);
    G->V = V; G->E = 0;
    return G;
  }
void GRAPHinsertE(Graph G, Edge e)
  { 
    if (G->adj[e.v][e.w] == maxWT) G->E++;
    G->adj[e.v][e.w] = e.wt; 
    G->adj[e.w][e.v] = e.wt; 
  }
-----
#include "GRAPH.h"
typedef struct node *link;
struct node { int v; double wt; link next; };
struct graph { int V; int E; link *adj; };
link NEW(int v, double wt, link next)
  { link x = malloc(sizeof *x);
    x->v = v; x->wt = wt; x->next = next;     
    return x;                         
  }
Graph GRAPHinit(int V)
  { int i;
    Graph G = malloc(sizeof *G);
    G->adj = malloc(V*sizeof(link));
    G->V = V; G->E = 0;
    for (i = 0; i < V; i++) G->adj[i] = NULL;
    return G;
  }
void GRAPHinsertE(Graph G, Edge e)
  { link t; 
    int v = e.v, w = e.w;
    if (v == w) return;
    G->adj[v] = NEW(w, e.wt, G->adj[v]);
    G->adj[w] = NEW(v, e.wt, G->adj[w]);
    G->E++;
  }
-----
static int fr[maxV];
#define P G->adj[v][w]
void GRAPHmstV(Graph G, int st[], double wt[])
  { int v, w, min; 
    for (v = 0; v < G->V; v++)
      { st[v] = -1; fr[v] = v; wt[v] = maxWT; }
    st[0] = 0; wt[G->V] = maxWT;
    for (min = 0; min != G->V; )
      {
        v = min; st[min] = fr[min];
        for (w = 0, min = G->V; w < G->V; w++)
          if (st[w] == -1)
            { 
              if (P < wt[w]) 
                { wt[w] = P; fr[w] = v; }
              if (wt[w] < wt[min]) min = w; 
            }
      }
  }
-----
#define GRAPHpfs GRAPHmst
static int fr[maxV];
static double *priority;
int less(int i, int j)
  { return priority[i] < priority[j]; }
#define P t->wt
void GRAPHpfs(Graph G, int st[], double wt[])
  { link t; int v, w; 
    PQinit(); priority = wt; 
    for (v = 0; v < G->V; v++)
      { st[v] = -1; fr[v] = -1; }
    fr[0] = 0; PQinsert(0);
    while (!PQempty())
      {
        v = PQdelmin(); st[v] = fr[v]; 
        for (t = G->adj[v]; t != NULL; t = t->next)
          if (fr[w = t->v] == -1) 
            { wt[w] = P; PQinsert(w); fr[w] = v; }
          else if ((st[w] == -1) && (P < wt[w])) 
            { wt[w] = P; PQdec(w); fr[w] = v; }
      }
  }
-----
void GRAPHmstE(Graph G, Edge mst[])
  { int i, k; Edge a[maxE]; 
    int E = GRAPHedges(a, G);
    sort(a, 0, E-1);
    UFinit(G->V);
    for (i= 0, k = 0; i < E && k < G->V-1; i++)
      if (!UFfind(a[i].v, a[i].w))
        {
          UFunion(a[i].v, a[i].w);
          mst[k++] = a[i];
        }
  }
-----
Edge nn[maxV], a[maxE];
void GRAPHmstE(Graph G, Edge mst[])
  { int h, i, j, k, v, w, N; Edge e;
    int E = GRAPHedges(a, G);
    for (UFinit(G->V); E != 0; E = N)
      {
        for (k = 0; k < G->V; k++) 
          nn[k] = EDGE(G->V, G->V, maxWT);
        for (h = 0, N = 0; h < E; h++)
          {
            i = find(a[h].v); j = find(a[h].w); 
            if (i == j) continue;
            if (a[h].wt < nn[i].wt) nn[i] = a[h];
            if (a[h].wt < nn[j].wt) nn[j] = a[h];
            a[N++] = a[h]; 
          }
        for (k = 0; k < G->V; k++) 
          {
            e = nn[k]; v = e.v; w = e.w;
            if ((v != G->V) && !UFfind(v, w)) 
              { UFunion(v, w); mst[k] = e; }
          }
      }
  }
-----
fixUp(Item a[], int k)
  {
    while (k > 1 && less(a[(k+d-2)/d], a[k]))
      { exch(a[k], a[(k+d-2)/d]); k = (k+d-2)/d; }
  }
fixDown(Item a[], int k, int N)
  { int i, j;
    while ((d*(k-1)+2) <= N)
      { j = d*(k-1)+2;
        for (i = j+1; (i < j+d) && (i <= N); i++)
          if (less(a[j], a[i])) j = i;
        if (!less(a[k], a[j])) break;
        exch(a[k], a[j]); k = j;
      }
  }

----------
CHAPTER 21. Shortest Paths
-----
#define GRAPHpfs GRAPHspt
#define P (wt[v] + t->wt)
void GRAPHpfs(Graph G, int s, int st[], double wt[])
  { int v, w;  link t;
    PQinit(); priority = wt;
    for (v = 0; v < G->V; v++)
      { st[v] = -1; wt[v] = maxWT; PQinsert(v); }
    wt[s] = 0.0; PQdec(s);
    while (!PQempty())
      if (wt[v = PQdelmin()] != maxWT)
        for (t = G->adj[v]; t != NULL; t = t->next)
          if (P < wt[w = t->v]) 
            { wt[w] = P; PQdec(w); st[w] = v; }
  }
-----
  void GRAPHspALL(Graph G);
double GRAPHspDIST(Graph G, int s, int t);
   int GRAPHspPATH(Graph G, int s, int t);
-----
void GRAPHdiameter(Graph G)
  { int v, w, vMAX = 0, wMAX = 0;
    double MAX = 0.0;
    GRAPHspALL(G);
    for (v = 0; v < G->V; v++)
      for (w = 0; w < G->V; w++)
        if (GRAPHspPATH(G, v, w) != G->V)
          if (MAX < GRAPHspDIST(G, v, w))
            { vMAX = v; wMAX = w; 
              MAX = GRAPHspDIST(G, v, w); }
    printf("Diameter is %f\n", MAX); 
    for (v = vMAX; v != wMAX; v = w)
      { printf("%d-", v);
        w = GRAPHspPATH(G, v, wMAX); }
    printf("%d\n", w);
  }
-----
static int st[maxV];
static double wt[maxV];
void GRAPHspALL(Graph G)
  { int v, w; Graph R = GRAPHreverse(G);
    G->dist = MATRIXdouble(G->V, G->V, maxWT);
    G->path = MATRIXint(G->V, G->V, G->V);
    for (v = 0; v < G->V; v++)
      {
        GRAPHpfs(R, v, st, wt);
        for (w = 0; w < G->V; w++)
          G->dist[w][v] = wt[w]; 
        for (w = 0; w < G->V; w++)
          if (st[w] != -1) G->path[w][v] = st[w]; 
      }
  }
double GRAPHspDIST(Graph G, int s, int t)
  { return G->dist[s][t]; }
int GRAPHspPATH(Graph G, int s, int t)
  { return G->path[s][t]; }
-----
void GRAPHspALL(Graph G)
  { int i, s, t;
    double **d = MATRIXdouble(G->V, G->V, maxWT);
    int **p = MATRIXint(G->V, G->V, G->V);
    for (s = 0; s < G->V; s++)
      for (t = 0; t < G->V; t++)
        if ((d[s][t] = G->adj[s][t]) < maxWT) 
          p[s][t] = t;
    for (i = 0; i < G->V; i++)
      for (s = 0; s < G->V; s++)
        if (d[s][i] < maxWT)
          for (t = 0; t < G->V; t++)
            if (d[s][t] > d[s][i]+d[i][t])
              { p[s][t] = p[s][i]; 
                d[s][t] = d[s][i]+d[i][t]; } 
    G->dist = d; G->path = p;
  }
-----
static int ts[maxV];
void GRAPHlpt(Graph G, int s, int st[], double wt[])
  { int i, v, w; link t;
    GRAPHts(G, ts);    
    for (v = ts[i = 0]; i < G->V; v = ts[i++])
      for (t = G->adj[v]; t != NULL; t = t->next)
        if (wt[w = t->v] < wt[v] + t->wt) 
          { st[w] = v; wt[w] = wt[v] + t->wt; }
  }
-----
void SPdfsR(Graph G, int s)
  { link u; int i, t; double wt;
    int **p = G->path; double **d = G->dist;
    for (u = G->adj[s]; u != NULL; u = u->next)
      {
        t = u->v; wt = u->wt;
        if (d[s][t] > wt)
          { d[s][t] = wt; p[s][t] = t; }
        if (d[t][t] == maxWT) SPdfsR(G, t); 
        for (i = 0; i < G->V; i++)
          if (d[t][i] < maxWT)
            if (d[s][i] > wt+d[t][i]) 
              { d[s][i] = wt+d[t][i]; p[s][i] = t; } 
      }
  }  
void GRAPHspALL(Graph G)
  { int v; 
    G->dist = MATRIXdouble(G->V, G->V, maxWT);
    G->path = MATRIXint(G->V, G->V, G->V);
    for (v = 0; v < G->V; v++)
      if (G->dist[v][v] == maxWT) SPdfsR(G, v);
  }
-----
#include <stdio.h>
#include "GRAPH.h"
#define Nmax 1000
main(int argc, char *argv[])
  { int i, s, t, N = atoi(argv[1]);
    double length[Nmax], start[Nmax];
    int st[Nmax];
    Graph G = GRAPHinit(N);
    for (i = 0; i < N; i++)
      scanf("%lf", &length[i]);
    while (scanf("%d %d", &s, &t) != EOF)
      GRAPHinsertE(G, EDGE(s, t, length[s]));
    GRAPHlpt(G, 0, st, start);
    for (i = 0; i < N; i++)
      printf("%3d %6.2f\n", i, start[i]);
  }
-----
void GRAPHbf(Graph G, int s, int st[], double wt[])
  { int v, w; link t; int N = 0;
    QUEUEinit(G->E);
    for (v = 0; v < G->V; v++)
      { st[v] = -1; wt[v] = maxWT; }
    wt[s] = 0.0; st[s] = 0; 
    QUEUEput(s); QUEUEput(G->V);
    while (!QUEUEempty())
      if ((v = QUEUEget()) == G->V)
        { if (N++ > G->V) return; QUEUEput(G->V); }
      else
        for (t = G->adj[v]; t != NULL; t = t->next)
          if (wt[w = t->v] > wt[v] + t->wt) 
            { wt[w] = wt[v] + t->wt; 
              QUEUEput(w); st[w] = v; }
  }


----------
CHAPTER 22. Network Flow
-----
#include <stdlib.h>
#include "GRAPH.h"
typedef struct node *link;
struct node 
  { int v; int cap; int flow; link dup; link next;};
struct graph 
  { int V; int E; link *adj; };
link NEW(int v, int cap, int flow, link next)
  { link x = malloc(sizeof *x);
    x->v = v; x->cap = cap; x->flow = flow; 
    x->next = next;     
    return x;                         
  }
Graph GRAPHinit(int V)
  { int i;
    Graph G = malloc(sizeof *G);
    G->adj = malloc(V*sizeof(link));
    G->V = V; G->E = 0;
    for (i = 0; i < V; i++) G->adj[i] = NULL;
    return G;
  }
void GRAPHinsertE(Graph G, Edge e)
  { int v = e.v, w = e.w;
    G->adj[v] = NEW(w, e.cap, e.flow, G->adj[v]);
    G->adj[w] = NEW(v, -e.cap, -e.flow, G->adj[w]);
    G->adj[v]->dup = G->adj[w];
    G->adj[w]->dup = G->adj[v];
    G->E++;
  }
-----
static int flowV(Graph G, int v)
  { link t; int x = 0;
    for (t = G->adj[v]; t != NULL; t = t->next)
      x += t->flow;
    return x;
  }
int GRAPHflow(Graph G, int s, int t)
  { int v, val = flowV(G, s);
    for (v = 0; v < G->V; v++)
      if ((v != s) && (v != t))
        if (flowV(G, v) != 0) return 0;
    if (val + flowV(G, t) != 0) return 0;
    if (val <= 0) return 0;
    return val;
  }
-----
static int wt[maxV];
#define Q (u->cap < 0 ? -u->flow : u->cap - u->flow)
int GRAPHpfs(Graph G, int s, int t, link st[])
  { int v, w, d = M; link u;
    PQinit(); priority = wt;
    for (v = 0; v < G->V; v++)
      { st[v] = NULL; wt[v] = 0; PQinsert(v); }
    wt[s] = M; PQinc(s); 
    while (!PQempty())
      {
        v = PQdelmax();
        if ((wt[v] == 0) || (v == t)) break;
        for (u = G->adj[v]; u != NULL; u = u->next)
          if (Q > 0)
            if (P > wt[w = u->v]) 
              { wt[w] = P; PQinc(w); st[w] = u; }
        wt[v] = M;
      }
    if (wt[t] == 0) return 0; 
    for (w = t; w != s; w = st[w]->dup->v)
      { u = st[w]; d = ( Q > d ? d : Q ); }
    return d;
  }
void GRAPHmaxflow(Graph G, int s, int t)
  { int x, d;
    link st[maxV]; 
    while ((d = GRAPHpfs(G, s, t, st)) != 0) 
      for (x = t; x != s; x = st[x]->dup->v)
        { st[x]->flow += d; st[x]->dup->flow -= d; }
  }
-----
static int h[maxV], wt[maxV];
#define P ( Q > wt[v] ? wt[v] : Q )
#define Q (u->cap < 0 ? -u->flow : u->cap - u->flow)
int GRAPHmaxflow(Graph G, int s, int t)
  { int v, w, x;  link u;
    GRAPHdist(G, t, h);
    GQinit(); 
    for (v = 0; v < G->V; v++) wt[v] = 0; 
    GQput(s); wt[s] = maxWT; wt[t] = -maxWT;
    while (!GQempty())
      {
        v = GQget();
        for (u = G->adj[v]; u != NULL; u = u->next)
          if (P > 0 && v == s || h[v] == h[u->v]+1)
            { 
              w = u->v; x = P;
              u->flow += x; u->dup->flow -= x; 
              wt[v] -= x; wt[w] += x;
              if ((w != s) && (w != t)) GQput(w);
            }
        if ((v != s) && (v != t))
          if (wt[v] > 0) { h[v]++; GQput(v); }
      }        
  }
-----
void insertSTlinks(Graph G, int s, int t)
  { int i, sd;
    for (i = 0; i < G->V; i++)
      if ((sd = G->sd[i]) >= 0)
        GRAPHinsertE(G, EDGE(s, i, sd, 0, 0));
    for (i = 0; i < G->V; i++)
      if ((sd = G->sd[i]) < 0)
        GRAPHinsertE(G, EDGE(i, t, -sd, 0, 0));
  }
void removeSTlinks(Graph G)
  { int i;
    for (i = 0; i < G->V; i++)
      G->adj[i] = G->adj[i]->next;
  }
int GRAPHfeasible(Graph G)
  { int s = G->V, t = G->V+1, sd = 0; link u;
    insertSTlinks(G, s, t); G->V += 2;
    GRAPHmaxflow(G, s, t);
    for (u = G->adj[s]; u != NULL; u = u->next)
      sd += u->cap - u->flow;
    for (u = G->adj[t]; u != NULL; u = u->next)
      sd += u->cap - u->flow;
    G->V -= 2; removeSTlinks(G);
    return sd;
  }
-----
#include <stdio.h>
#include "GRAPH.h"
main(int argc, char *argv[])
  { Graph G; int i, v, w, E, V = atoi(argv[1]);
    G = GRAPHinit(2*V+2);
    for (i = 1; i <= V; i++)
      GRAPHinsertE(G, EDGE(0, i, 1, 0));
    while (scanf("%d %d", &v, &w) != EOF)
      GRAPHinsertE(G, EDGE(v, w, 1, 0));
    for (i = V+1; i <= V+V; i++)
      GRAPHinsertE(G, EDGE(i, V+V+1, 1, 0));
    if (GRAPHmaxflow(G, 0, V+V+1) == 0) return;
    E = GRAPHedges(a, G);
    for (i = 0; i < E; i++)
      if ((a[i].v != 0) && (a[i].w != V+V+1))
        if (a[i].flow == 1)
          printf("%d-%d\n", a[i].v, a[i].w);
  }
-----
int GRAPHcost(Graph G)
  { int i; link u; int cost = 0;
    for (i = 0; i < G->V; i++)
      for (u = G->adj[i]; u != NULL; u = u->next)
        if ((u->cap > 0) && (u->cost != C))
          cost += (u->flow)*(u->cost);
    return cost;
  }
-----
void addflow(link u, int d)
  { u->flow += d; u->dup->flow -=d; }
int GRAPHmincost(Graph G, int s, int t)
  { int d, x, w; link u, st[maxV]; 
    GRAPHmaxflow(G, s, t);
    while ((x = GRAPHnegcycle(G, st)) != -1)
      {
        u = st[x]; d = Q;
        for (w = u->dup->v; w != x; w = u->dup->v)
          { u = st[w]; d = ( Q > d ? d : Q ); }
        u = st[x]; addflow(u, d);
        for (w = u->dup->v; w != x; w = u->dup->v)
          { u = st[w]; addflow(u, d); }
      }
    return GRAPHcost(G);
  }
-----
#define ST(i) st[i]->dup->v
static int valid, phi[maxV];
int phiR(link st[], int v)
  {
    if (ST(v) == v) 
      { mark[v] = valid; return -C; }
    if (mark[v] != valid) 
      phi[v] =phiR(st, ST(v)) - st[v]->cost;
    mark[v] = valid;
    return phi[v]; 
  }
-----
int lca(link st[], int u, int v)
  { int i, j;
    mark[u] = ++valid; mark[v] = valid;
    while (u != v)
    { 
      u = ST(u); v = ST(v); 
      if (u != ST(u) && mark[u] == valid) return u;
      mark[u] = valid; 
      if (v != ST(v) && mark[v] == valid) return v;
      mark[v] = valid; 
    }
    return u;
  }      
link augment(link st[], link x)
{ link u, cyc[maxV]; int d, N;
  int t, i = x->v, j = x->dup->v;
  t = lca(st, i, j);
  cyc[0] = x; N = 1;
  while (i != t) 
    { cyc[N++] = st[i]->dup; i = ST(i); }
  while (j != t) 
    { cyc[N++] = st[j]; j = ST(j); }
  for (i = 0, d = C; i < N; i++)
    { u = cyc[i]; d = Q > d ? d : Q; }
  for (i = 0; i < N; i++) addflow(cyc[i], d);
  for (i = 0; i < N-1; i++)
    { u = cyc[N-1-i]; if (Q == 0) return u; }
  }
-----
int onpath(link st[], int a, int b, int c)
  { int i;
    for (i = a; i != c; i = ST(i))
      if (i == b) return 1;
    return 0;
  }
int reverse(link st[], int u, int x)
  { int i;
    while (i != st[x]->v)
      { i = st[u]->v; st[i] = st[u]->dup; u = i; }
  }
int update(link st[], link w, link y)
  { int t, u = y->v, v = y->dup->v, x = w->v;
    if (st[x] != w->dup) x = w->dup->v;
    t = lca(st, u, v);
    if (onpath(st, u, x, t)) 
      { st[u] = y; reverse(st, u, x); return; }
    if (onpath(st, v, x, t)) 
      { st[v] = y->dup; reverse(st, v, x); return; }
  }
-----
#define R(u)  u->cost - phi[u->dup->v] + phi[u->v]
void addflow(link u, int d)
  { u->flow += d; u->dup->flow -=d; }
int GRAPHmincost(Graph G, int s, int t)
  { int v; link u, x, st[maxV]; 
    GRAPHinsertE(G, EDGE(s, t, M, M, C));
    initialize(G, s, t, st); 
    for (valid = 1; valid++; )
    {
      for (v = 0; v < G->V; v++) 
        phi[v] = phiR(st, v);
      for (v = 0, x = G->adj[v]; v < G->V; v++)
        for (u = G->adj[v]; u != NULL; u = u->next)
          if (Q > 0)
            if (R(u) < R(x)) x = u;
      if (R(x) == 0) break;
      update(st, augment(st, x), x);
    }
    return GRAPHcost(G);
  }
-----
int R(link st[], link u)
  { return u->cost 
      - phiR(st, u->dup->v) + phiR(st, u->v); }
int GRAPHmincost(Graph G, int s, int t)
  { int v, old = 0; link u, x, st[maxV]; 
    GRAPHinsertE(G, EDGE(s, t, M, M, C));
    initialize(G, s, t, st); 
    for (valid = 1; valid != old; old = valid)
      for (v = 0; v < G->V; v++)
        for (u = G->adj[v]; u != NULL; u = u->next)
          if ((Q > 0) && (R(st, u) < 0))
          { update(st, augment(st, u), u); valid++; }
    return GRAPHcost(G);
  }
-----
