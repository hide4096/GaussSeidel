#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MINIMAL_DIFF 0.000000000001
#define LOOP_ABORT 1000000

//まとめて解放する
int allfree(int** arr,int n,double* bx,double* x){
    for(int i=0;i<n;i++){
        free(arr[i]);
    }
    free(arr);
    free(bx);
    free(x);
}

//発散するか確認（対角優位性の計算）
int isDiverge(int** arr,int n){
    for(int i=0;i<n;i++){
        int ii = abs(arr[i][i]);
        int ij = 0;
        for(int j=0;j<n;j++){
            if(i!=j) ij+=abs(arr[j][i]);
        }
        if(ii < ij) return 1;
    }
    return 0;
}

//ガウスザイデルの計算
int GaussSeidel(int** arr,int n,double* bx,double* x){
    //ガウスザイデル法で計算
    int cnt = 0;
    int isDeverged = 0;
    while(!isDeverged){
        
    #ifdef PRINTCSV
        //CSV形式で経過を吐く
        printf("%d,",cnt);
        for(int i=0;i<n;i++){
      	    printf("%.10f,",x[i]);
        }
        printf("\n");
    #endif

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
        //一定数のループで打ち切る
        if(cnt >= LOOP_ABORT) return -2;
        for(int i=0;i<n;i++){
            //非数が出たら打ち切る
            if(isnan(x[i])) return -1;
            //各々の解の差分が一定値未満で打ち切る
            double diff = fabs(bx[i] - x[i]);
            if(diff > MINIMAL_DIFF) break;
            if(i >= n-1) isDeverged = 1;
        }
    }
    return cnt;
}


int main(){
    int n;
    int** arr = NULL;
    double* x = NULL;
    double* bx = NULL;
    
    //係数行列のサイズを入力
    scanf("%d",&n);
    if(n <= 0) return 1;

    //解のメモリを確保
    x = (double*)malloc(sizeof(double)*n);
    bx = (double*)malloc(sizeof(double)*n);
    if(x == NULL || bx == NULL){
        allfree(arr,n,x,bx);
        return 1;
    }

    for(int i=0;i<n;i++) x[i] = 1.0;    //初期値は1.0固定
    
    //行列のメモリを確保
    arr = (int**)malloc(sizeof(int*)*(n+1));
    if(arr == NULL){
        allfree(arr,n,x,bx);
        return 1;
    }
    for(int i=0;i<n+1;i++){
        arr[i] = (int*)malloc(sizeof(int)*n);
        if(arr[i] == NULL){
            allfree(arr,n,x,bx);
            return 1;
        }
    }
    
    //行列に係数を代入
    for(int i=0;i<(n+1)*n;i++){
        scanf("%d",&arr[i%(n+1)][i/(n+1)]);
    }

    //収束の確認と計算
    if(isDiverge(arr,n)) printf("[warn]This matrix will diverge.\r\n");
    int cnt = GaussSeidel(arr,n,bx,x);

    //結果を出力
    if(cnt < 0){
        if(cnt == -1) printf("[fail]This matrix diverged.\r\n");
        else if(cnt == -2) printf("[fail]Couldn't solve in %d Loop.\r\n",LOOP_ABORT);
        else printf("[fail]Unknown error.\r\n");
    }else{
        printf("[ OK ]finished\r\n");
        printf("\r\nLoop:%d\r\n",cnt);
        for(int i=0;i<n;i++){
      	    printf("x%d:%.10f\r\n",i+1,x[i]);
        }
    }

    allfree(arr,n,x,bx);
}
