#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MINIMAL_DIFF 0.000000000001
#define LOOP_ABORT 1000000

int **arr = NULL;
double *x = NULL;
double *bx = NULL;

int allfree(int n){
    for(int i=0;i<n;i++){
        free(arr[i]);
    }
    free(arr);
    free(bx);
    free(x);
}

int main(){
    int n;
    

    //係数行列のサイズを入力
    scanf("%d",&n);
    if(n <= 0) return 1;

    //解のメモリを確保
    x = (double*)malloc(sizeof(double)*n);
    bx = (double*)malloc(sizeof(double)*n);
    if(x == NULL || bx == NULL){
        allfree(0);
        return 1;
    }

    for(int i=0;i<n;i++) x[i] = 1.0;    //初期値は1.0固定
    
    //行列のメモリを確保
    arr = (int**)malloc(sizeof(int*)*(n+1));
    if(arr == NULL){
        allfree(n);
        return 1;
    }
    for(int i=0;i<n+1;i++){
        arr[i] = (int*)malloc(sizeof(int)*n);
        if(arr[i] == NULL){
            allfree(n);
            return 1;
        }
    }
    
    //行列に係数を代入
    for(int i=0;i<(n+1)*n;i++){
        scanf("%d",&arr[i%(n+1)][i/(n+1)]);
    }

    //ガウスザイデル法で計算
    int cnt = 0;
    int isDeverged = 0;
    while(!isDeverged){
        
        //CSV形式で経過を吐く
        printf("%d,",cnt);
        for(int i=0;i<n;i++){
      	    printf("%.10f,",x[i]);
        }
        printf("\n");

        //各行で計算する
        for(int j=0;j<n;j++){
            double tmp = (double)arr[n][j];
            for(int k=0;k<n;k++){
                if(k!=j) tmp-=(double)arr[k][j]*x[k];
            }
            bx[j] = x[j];
            x[j]=tmp/(double)arr[j][j];
        }

        //収束の確認とループカウント
        cnt++;
        if(cnt >= LOOP_ABORT) break;    //一定数のループで打ち切る
        for(int i=0;i<n;i++){
            double diff = fabs(bx[i] - x[i]);
            if(diff > MINIMAL_DIFF) break;  //差分が一定値未満で打ち切る
            if(i >= n-1) isDeverged = 1;
        }
    }
    //結果を出力
    printf("\r\nLoop:%d\r\n",cnt);
    for(int i=0;i<n;i++){
      	printf("x%d:%.10f\r\n",i+1,x[i]);
    }

    allfree(n);
}
