

//Student ID: 51803090025
//Name: Eduardo Wang Zheng

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  

#include <sys/types.h>  
#include <sys/stat.h>  

#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cctype>
#define MAX 507

using namespace std;


FILE *fp;                                     /* 定义文件指针*/

    


class WF
{
public:
    //产生式左部
    string left;
    //产生式右部
    vector<string> right;
    WF ( const string& str )
    {
        left = str;
    }
    void insert ( char str[] )
    {
        right.push_back(str);
    }
    void print ( )
    {
           

        fprintf ( fp, "%s%s%s" , left.c_str() ,"->",  right[0].c_str() );
        for ( int i = 1 ; i < right.size() ; i++ )
            fprintf ( fp, "%s%s" ,"|",  right[i].c_str() );
       // puts("");
                  fprintf(fp, "%s\n", "");

         
    }
};

char relation[MAX][MAX];
vector<char> VT;
vector<WF> VN_set;
map<string,int> VN_dic;
set<char> first[MAX];
set<char> last[MAX];
int used[MAX];
int vis[MAX];


void dfs (  int x )
{
    if ( vis[x] ) return;
    vis[x] = 1;
    //产生式左部
    string& left = VN_set[x].left;
    for ( int i = 0 ; i < VN_set[x].right.size() ; i++ )
    {
        //产生式右部
        string& str = VN_set[x].right[i];
        //遇到非终结符号
        if ( isupper(str[0]) )
        {
            int y = VN_dic[str.substr(0,1)]-1;
            
            //非终结符号后面紧跟着一个终结符号
            if ( str.length() > 1 && !isupper(str[1] ) )
                first[x].insert ( str[1] );
            
            //对于形如P->Q...的产生式，把firstvt(Q)加入firstvt(P)
            dfs ( y );
            set<char>::iterator it = first[y].begin();
            for ( ; it!= first[y].end() ; it++ )
                first[x].insert ( *it );
        }
        //是终结符号加入firstvt集合
        else
            first[x].insert ( str[0] );
    }
}

void make_first ( )
{
   

       
          
    memset ( vis , 0 , sizeof ( vis ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
        if ( vis[i] ) continue;
        else dfs ( i );
#define DEBUG
#ifdef DEBUG
       fprintf ( fp, "%s\n" , "------------FIRSTVT集-------------------");
    //puts("------------FIRSTVT集-------------------");
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        fprintf ( fp, "%s%s" , VN_set[i].left.c_str() , " :");
        set<char>::iterator it = first[i].begin();
        for ( ; it!= first[i].end() ; it++ )
            fprintf ( fp, "%c " , *it );
       // puts ("" );
               fprintf (fp, "%s\n", "");
    }
#endif


 
}

void dfs1 ( int x )
{
    if ( vis[x] ) return;
    vis[x] = 1;
    string& left = VN_set[x].left;
    for ( int i = 0 ; i < VN_set[x].right.size() ; i++ )
    {
        string& str = VN_set[x].right[i];
        int n = str.length() -1;
        if ( isupper(str[n] ) )
        {
            int y = VN_dic[str.substr(n,1)]-1;
            
            //非终结符号前面紧跟着一个终结符号
            if ( str.length() > 1 && !isupper(str[n-1]) )
                last[x].insert ( str[n-1] );
            
            //对于形如P->...Q的产生式，把lastvt(Q)加入lastvt(P)
            dfs1 ( y );
            set<char>::iterator it = last[y].begin();
            for ( ; it != last[y].end() ; it++ )
                last[x].insert ( *it );
        }
        else
            last[x].insert ( str[n] );
    }
}


void make_last ( )
{
   



    memset ( vis , 0 , sizeof ( vis ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
        if ( vis[i] ) continue;
        else dfs1 ( i );
#define DEBUG
#ifdef DEBUG
    //puts("--------------LASTVT集---------------------");
          fprintf (fp,  "%s\n", "--------------LASTVT集---------------------");
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        fprintf (fp,  "%s%s" , VN_set[i].left.c_str(), ": " );
        set<char>::iterator it = last[i].begin();
        for ( ; it!= last[i].end() ; it++ )
            fprintf (fp,  "%c " , *it );
        //puts ("" );
                 fprintf (fp, "%s\n", "");
    }
#endif


}

void make_table ( )
{
    

       
          
   

 
    for ( int i = 0 ; i < MAX ; i++ )
        for ( int j = 0 ; j < MAX ; j++ )
            relation[i][j] = ' ';
    for ( int i = 0 ; i < VN_set.size() ; i++ )
        for ( int j = 0 ; j < VN_set[i].right.size() ; j++ )
        {
            string& str = VN_set[i].right[j];
            for ( int k = 0 ; k < str.length()-1 ; k++ )
            {
                //遇到两个终结符a和b紧挨在一起的情况，则添加关系a=b
                if ( !isupper(str[k]) && !isupper(str[k+1]) )
                    relation[str[k]][str[k+1]] = '=';
                
                //遇到一个终结符a后面紧跟着一个非终结符P的情况，则对于firstvt(P)中的每一个终结符b，添加关系a<b
                if ( !isupper(str[k]) && isupper(str[k+1]) )
                {
                    int x = VN_dic[str.substr(k+1,1)]-1;
                    set<char>::iterator it = first[x].begin();
                    for ( ; it != first[x].end() ; it++ )
                        relation[str[k]][*it] = '<';
                }
                
                //遇到一个非终结符P后面紧跟着一个终结符a的情况，则对于lastvt(P)中的每一个终结符b，添加关系b>a
                if ( isupper(str[k]) && !isupper(str[k+1]) )
                {
                    int x = VN_dic[str.substr(k,1)]-1;
                    set<char>::iterator it = last[x].begin();
                    for ( ; it != last[x].end() ; it++ )
                        relation[*it][str[k+1]] = '>';
                }
                
                //当遍历的位置后面至少有2个元素时，如果遇到两个终结符a和b中间夹着一个非终结符P的情况(形如aPb)，添加关系a=b
                if ( k > str.length()-2 ) continue;
                if ( !isupper(str[k]) && !isupper(str[k+2]) && isupper(str[k+1]) )
                    relation[str[k]][str[k+2]] = '=';
            }
        }
#define DEBUG
#ifdef DEBUG
    for ( int i = 0 ; i < VT.size()*5 ; i++ )
        fprintf (fp, "%s", "-");
    fprintf (fp, "%s",  "算符优先关系表" );
    for ( int i = 0 ; i < VT.size()*5 ; i++ )
        fprintf (fp, "%s", "-");
    //puts("");
          fprintf (fp, "%s\n", "");
    fprintf (fp,  "%s%8s%s" , "|", "", "|" );
    for ( int i = 0 ; i < VT.size() ; i++ )
        fprintf (fp,  "%5c%5s" , VT[i] , "|" );
    //puts ("");
            fprintf (fp, "%s\n", "");
    for ( int i = 0 ; i < (VT.size()+1)*10 ; i++ )
        fprintf (fp, "%s", "-");
   // puts("");
              fprintf (fp, "%s\n", "");
    for ( int i = 0 ; i < VT.size() ; i++ )
    {
        fprintf (fp,  "%s%4c%5s" ,  "|" , VT[i] , "|");
        for ( int j = 0 ; j < VT.size() ; j++ )
            fprintf (fp,  "%5c%5s" , relation[VT[i]][VT[j]] , "|" );
        //puts ("");
               fprintf (fp, "%s\n", "");
        for ( int i = 0 ; i < (VT.size()+1)*10 ; i++ )
            fprintf (fp, "%s", "-");
        //puts("");
           fprintf (fp, "%s\n", "");
    }
#endif



}


int main ( )
{
if( ( fp = fopen("out.txt", "w") ) == NULL){    /* 打开文件 */
           printf("File open error!\n");
           exit(0);
    }

  
  

  
     
    int n;
    char s[MAX];
    ifstream myfile("test.txt");
    //ofstream outfile("out.txt", ios::app);
   string temp;
    if (!myfile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    while(getline(myfile,temp))
    {
memset ( used , 0 , sizeof ( used ) );
            int i;
            for(i=0; i<temp.length();i++)
                     s[i]=temp[i];
            int len = strlen(s),j;
            for ( j = 0 ; j < len ; j++ )
                if ( s[j] == '-' )
                    break;
            s[j] = 0;
            if ( !VN_dic[s] )
            {
                //构造存放产生式的容器
                VN_set.push_back ( WF(s) );
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert ( s+j+2 );
        
           //将产生式左部的所有终结符存入终结符集
            for ( int k = 0 ; k < j; k++ )
                if ( !isupper(s[k] )&& s[k]!='|' )
                {
                    if ( used[s[k]] ) continue;
                    used[s[k]] = 1;
                    VT.push_back ( s[k] );
                }
        
           //将产生式右部的所有终结符存入终结符集
            for ( int k = j+2 ; k < len; k++ )
                if ( !isupper(s[k] ) && s[k]!='|')
                {
                    if ( used[s[k]] ) continue;
                    VT.push_back ( s[k] );
                    used[s[k]] = VT.size();
                }
        //outfile << temp;
        //outfile << endl;
    }


    myfile.close();
    //outfile.close();
  


#define DEBUG
#ifdef DEBUG

 













        fprintf (fp,  "%s\n", "************VT集*******************");
        for ( int i = 0 ; i < VT.size() ; i++ )
            fprintf (fp,  "%c " , VT[i] );
        //puts ("");
              fprintf (fp,  "%s\n", "");
        //puts("*************产生式*****************");
             fprintf (fp,  "%s\n", "************产生式******************");
        for ( int i = 0 ; i < VN_set.size() ; i++ )
            VN_set[i].print();
        //puts("************************************");
            
               fprintf (fp,  "%s\n", "************************************");

        //puts ("************VT集*******************");
       
       // puts ("");
      
       // puts("*************产生式*****************");
       
        
        //puts("************************************");



        
#endif





        make_first();
        make_last();
        make_table();

 if( fclose( fp ) ){                         /* 关闭文件 */
        printf( "Can not close the file!\n" );
        exit(0);
    }




       
    return 0;





















  /*  while ( ~scanf ( "%d" , &n ) )
    {
        memset ( used , 0 , sizeof ( used ) );
        for ( int i = 0 ; i < n ; i++ )
        {
            scanf ( "%s" , s );
            int len = strlen(s),j;
            for ( j = 0 ; j < len ; j++ )
                if ( s[j] == '-' )
                    break;
            s[j] = 0;
            if ( !VN_dic[s] )
            {
                VN_set.push_back ( WF(s) );
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert ( s+j+2 );
            for ( int k = 0 ; k < j; k++ )
                if ( !isupper(s[k] ) )
                {
                    if ( used[s[k]] ) continue;
                    used[s[k]] = 1;
                    VT.push_back ( s[k] );
                }
            for ( int k = j+2 ; k < len; k++ )
                if ( !isupper(s[k] ) )
                {
                    if ( used[s[k]] ) continue;
                    VT.push_back ( s[k] );
                    used[s[k]] = VT.size();
                }
        }*/

    
}

