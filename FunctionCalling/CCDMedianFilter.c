/*          Copyright(C)2008 by FUJI FILM Co.,Ltd.
********************************************************
*       <<CCD data Correction Software Library>>
*ファイル名称:CCDMedianFilter.c
*定義関数名称:ＣＣＤ補正処理ソフト(メディアンフィルター処理)
*
*本補正処理は、ファイル先頭を画像の左上としています。
*  上記に基づき、パラメータ名が決められています。
*
*履歴        :create 2006.07.26  YSK 宮崎   FPD1化
*            :update 2008.05.19  HSK 伊藤	SELENE向けラッパ関数定義
*             
********************************************************/

/*******************************
* INCLUDE
*******************************/

#include  <stdlib.h>
#include  <string.h>
#include  <stdio.h>
#include  <malloc.h>
#include  <math.h>
#include  <time.h>
#include  "CCDCommon.h"


/*******************************
* リテラル定義
*******************************/

/* メディアンフィルター処理切り替え値 */
/*      MEDIAN_CHG_COFFより大きい時が、端は平均値にて拡張   */
/*      MEDIAN_CHG_COFF以下時が、端はコピー値にて拡張       */
/*#define MEDIAN_CHG_COFF     21*/
/*  仕様変更により、端処理は全て平均データ                  */
#if 1
#define MEDIAN_CHG_COFF     0
#else
#define MEDIAN_CHG_COFF     21
#endif


/*******************************
* ラッパ関数定義
* update 2008.05.19  HSK 伊藤	SELENE向けラッパ関数定義
*******************************/


extern void* Mem_Alloc(unsigned long ulSize)
{
    return (void*)malloc(ulSize);
}

extern void  Mem_Free(void* buf)
{
    free(buf);
}


/******************************
* 関数パラメータ構造体         
*******************************/

typedef    void*    CCD_HANDLE;

typedef struct {
    int             index;
    unsigned short  data;
} MidData;

typedef struct {
    MidData         *List;
    int             max;
    int             cnt;
} SortSelf;

typedef struct {
    int             index;
    double          data;
} MidDatad;

typedef struct {
    MidDatad        *List;
    int             max;
    int             cnt;
} SortSelfd;


/*******************************
* 関数ProteType
********************************/

CCD_HANDLE CreateGetMidData(int Cs);
CCD_HANDLE CreateGetMidDatad(int Cs);
int  UpDownSort(MidData *List, int cnt, int end, int type);
int  UpDownSortd(MidDatad *List, int cnt, int end, int type);
int  SortData(MidData *List, int cnt, unsigned short Din, int max);
int  SortDatad(MidDatad *List, int cnt, double Din, int max);
unsigned short GetMidData(CCD_HANDLE handle, unsigned short Din);
double GetMidDatad(CCD_HANDLE handle, double Din);
int  InitMidData(CCD_HANDLE handle);
int  InitMidDatad(CCD_HANDLE handle);
void DeleteGetMidData(CCD_HANDLE handle);
void DeleteGetMidDatad(CCD_HANDLE handle);

#define NEW_MEDIAN_DEFAULT  (-65536)


/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:CreateGetMidData
*機能概要:メディアンフィルタ処理オブジェクト作成
*引数    :int               Cs     i マスクサイズ
*戻り値  :CCD_HANDLE        handle o 本オブジェクトアクセスハンドル
*機能説明:リスト付ソート処理オブジェクトの作成を行う。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*         update 2006.10.16  YSK 中野   コンパイルワーニング対応(IntelCompiler使用時)
*				 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

CCD_HANDLE CreateGetMidData(int Cs)
{
    SortSelf *self = NULL;
/*    int      i = 0; */

        /* パラメータチェック */
    if (Cs <= 0) {
        return (NULL);
    }

/* EditY 2007.03.16-ST */
/*
    self = (SortSelf *)malloc(sizeof(SortSelf));
*/
    self = (SortSelf *)Mem_Alloc(sizeof(SortSelf));
/* EditY 2007.03.16-ED */
    if (self != NULL) {
        memset((void *)self, 0, sizeof(SortSelf));
/* EditY 2007.03.16-ST */
/*
        self->List = (MidData *)malloc(Cs * sizeof(MidData));
*/
        self->List = (MidData *)Mem_Alloc(Cs * sizeof(MidData));
/* EditY 2007.03.16-ED */
        if (self->List != NULL) {
            self->max = Cs;
            InitMidData(self);
        } else {
/* EditY 2007.03.16
            free (self);
*/
		    Mem_Free (self);
            self = NULL;
        }
    }
    return (self);
}


/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:CreateGetMidDatad
*機能概要:メディアンフィルタ処理オブジェクト作成  浮動少数版
*引数    :int               Cs     i マスクサイズ
*戻り値  :CCD_HANDLE        handle o 本オブジェクトアクセスハンドル
*機能説明:リスト付ソート処理オブジェクトの作成を行う。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*         update 2006.10.16  YSK 中野   コンパイルワーニング対応(IntelCompiler使用時)
*				 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

CCD_HANDLE CreateGetMidDatad(int Cs)
{
    SortSelfd   *self = NULL;
/*     int         i = 0; */

        /* パラメータチェック */
    if (Cs <= 0) {
        return (NULL);
    }

/* EditY 2007.03.16-ST */
/*
    self = (SortSelfd *)malloc(sizeof(SortSelfd));
*/
    self = (SortSelfd *)Mem_Alloc(sizeof(SortSelfd));
/* EditY 2007.03.16-ED */
    if (self != NULL) {
        memset((void *)self, 0, sizeof(SortSelfd));
/* EditY 2007.03.16-ST */
/*
        self->List = (MidDatad *)malloc(Cs * sizeof(MidDatad));
*/
        self->List = (MidDatad *)Mem_Alloc(Cs * sizeof(MidDatad));
/* EditY 2007.03.16-ED */
        if (self->List != NULL) {
            self->max = Cs;
            InitMidDatad(self);
        } else {
/* EditY 2007.03.16
            free (self);
*/
		    Mem_Free (self);
            self = NULL;
        }
    }
    return (self);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:UpDownSort
*機能概要:メディアンフィルタ処理(指定位置ソート処理)
*           指定位置から降順or昇順でデータソート
*引数    :MidData           *List  i 本オブジェクトアクセスハンドル
*         int               cnt    i リストへの挿入位置Index
*         int               end    i リスト最終Index
*         int               type   i ソート方向
*                                       0 : 昇順  (0 -> n)
*                                       1 : 降順  (n -> 0)
*戻り値  :int   ソート実施   (CCDOK)
*               ソート未実施 (CCDNG)
*機能説明:メディアンフィルタ―のソートを行なう。
*備考    :ソート最大が"100"である
*           本モジュールはパラメータチェックを行わない。
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int UpDownSort(MidData *List, int cnt, int end, int type)
{
    int         i = 0;
    int         ret = CCDNG;
    MidData     buf;
    int         add = 0;

    if (type == 0) {    /* 昇順の場合 */
        end = end - cnt - 1;
        if (end < 0) {
            end = 0;
        }
        if (end > 100) {
            end = 100;
        }
        add = 1;
    } else {            /* 降順の場合 */
        end = cnt - 0;
        if (end < 0) {
            end = 0;
        }
        if (end > 100) {
            end = 100;
        }
        add = -1;
    }

    for (i = 0; i < end; i++) {
        if ((List[cnt+add].data <= List[cnt].data) && (type == 1)) {
            break;
        }
        if ((List[cnt+add].data >= List[cnt].data) && (type == 0)) {
            break;
        }
        ret = CCDOK;
        buf.index = List[cnt+add].index;
        buf.data  = List[cnt+add].data;
        List[cnt+add].index = List[cnt].index;
        List[cnt+add].data  = List[cnt].data;
        List[cnt].index = buf.index;
        List[cnt].data  = buf.data;
        cnt += add;
    }

    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:UpDownSortd
*機能概要:メディアンフィルタ処理(指定位置ソート処理) 浮動少数版
*           指定位置から降順or昇順でデータソート
*引数    :MidDatad          *List  i 本オブジェクトアクセスハンドル
*         int               cnt    i リストへの挿入位置Index
*         int               end    i リスト最終Index
*         int               type   i ソート方向
*                                       0 : 昇順  (0 -> n)
*                                       1 : 降順  (n -> 0)
*戻り値  :int   ソート実施   (CCDOK)
*               ソート未実施 (CCDNG)
*機能説明:メディアンフィルタ―のソートを行なう。
*備考    :ソート最大が"100"である
*           本モジュールはパラメータチェックを行わない。
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int UpDownSortd(MidDatad *List, int cnt, int end, int type)
{
    int         i = 0;
    int         ret = CCDNG;
    MidDatad    buf;
    int         add = 0;

    if (type == 0) {    /* 昇順の場合 */
        end = end - cnt - 1;
        if (end < 0) {
            end = 0;
        }
        if (end > 100) {
            end = 100;
        }
        add = 1;
    } else {            /* 降順の場合 */
        end = cnt - 0;
        if (end < 0) {
            end = 0;
        }
        if (end > 100) {
            end = 100;
        }
        add = -1;
    }

    for (i = 0; i < end; i++) {
        if ((List[cnt+add].data <= List[cnt].data) && (type == 1)) {
            break;
        }
        if ((List[cnt+add].data >= List[cnt].data) && (type == 0)) {
            break;
        }
        ret = CCDOK;
        buf.index = List[cnt+add].index;
        buf.data  = List[cnt+add].data;
        List[cnt+add].index = List[cnt].index;
        List[cnt+add].data  = List[cnt].data;
        List[cnt].index = buf.index;
        List[cnt].data  = buf.data;

        cnt += add;
    }

    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:SortData
*機能概要:メディアンフィルタ処理データソート
*引数    :MidData           *List  i 本オブジェクトアクセスハンドル
*         int               cnt    i リストへの挿入位置Index
*         unsigned short    Din    i 新しくリストに追加するデータ
*         int               max    i リストIndex最大数
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:メディアンフィルタ―のソートを行なう。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int SortData(MidData *List, int cnt, unsigned short Din, int max)
{
    int     i = 0;
    int     ret = CCDNG;

        /* パラメータチェック */
    if ((cnt < 0) || (max <= 0) || (List == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }

        /* 新しいデータの挿入先の検索 */
    for (i = 0; i < max; i++) {
        if (List[i].index == cnt) {
                /* 挿入先発見 */
            break;
        }
    }

        /* 新しいデータを挿入 */
    List[i].data = Din;

        /* 対象位置からIndex番号の小さい方へソート          */
    ret = UpDownSort(List, i, 0, 1);

    if (ret != CCDOK) { /* 小さい方のソートだけでは不十分   */
            /* 対象位置からIndex番号の多きい方へソート      */
        ret = UpDownSort(List, i, max, 0);
    }

    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:SortDatad
*機能概要:メディアンフィルタ処理データソート  浮動少数版
*引数    :MidDatad          *List  i 本オブジェクトアクセスハンドル
*         int               cnt    i リストへの挿入位置Index
*         double            Din    i 新しくリストに追加するデータ
*         int               max    i リストIndex最大数
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:メディアンフィルタ―のソートを行なう。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int SortDatad(MidDatad *List, int cnt, double Din, int max)
{
    int     i = 0;
    int     ret = CCDNG;

        /* パラメータチェック */
    if ((cnt < 0) || (max <= 0) || (Din < 0) || (List == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }

        /* 新しいデータの挿入先の検索 */
    for (i = 0; i < max; i++) {
        if (List[i].index == cnt) {
                /* 挿入先発見 */
            break;
        }
    }

        /* 新しいデータを挿入 */
    List[i].data = Din;

        /* 対象位置からIndex番号の小さい方へソート          */
    ret = UpDownSortd(List, i, 0, 1);

    if (ret != CCDOK) { /* 小さい方のソートだけでは不十分   */
            /* 対象位置からIndex番号の多きい方へソート      */
        ret = UpDownSortd(List, i, max, 0);
    }

    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:GetMidData
*機能概要:メディアンフィルタ処理中間データ抽出処理
*引数    :CCD_HANDLE        handle i 本オブジェクトアクセスハンドル
*         unsigned short    Din    i 新しくリストに追加するデータ
*戻り値  :unsigned short    Dout   o ﾒﾃﾞｨｱﾝﾌｨﾙﾀ処理結果
*機能説明:リスト内の中間データの抽出を行なう。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

unsigned short GetMidData(CCD_HANDLE handle, unsigned short Din)
{
    SortSelf *self = (SortSelf *)handle;
    unsigned short  data = 0xFFFF;

        /* パラメータチェック */
    if (self == NULL) {
        return (data);
    }

        /* 新しいデータを付加してソート */
    SortData(self->List, self->cnt, Din, self->max);

        /* 次のインデックス番号作成 */
    self->cnt++;
    if (self->cnt >= self->max) {
        self->cnt = 0;
    }

        /* 中間データ抽出 */
        /*   中間画素は、"(self->max)/2+1"画素目 */
        /*   配列では、"(self->max)/2"番目となる */
    data = self->List[(self->max)/2].data;

    return (data);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:GetMidDatad
*機能概要:メディアンフィルタ処理中間データ抽出処理  浮動少数版
*引数    :CCD_HANDLE        handle i 本オブジェクトアクセスハンドル
*         double            Din    i 新しくリストに追加するデータ
*戻り値  :double            Dout   o ﾒﾃﾞｨｱﾝﾌｨﾙﾀ処理結果
*機能説明:リスト内の中間データの抽出を行なう。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

double GetMidDatad(CCD_HANDLE handle, double Din)
{
    SortSelfd   *self = (SortSelfd *)handle;
    double      data = 0xFFFF;

        /* パラメータチェック */
    if (self == NULL) {
        return (data);
    }

        /* 新しいデータを付加してソート */
    SortDatad(self->List, self->cnt, Din, self->max);

        /* 次のインデックス番号作成 */
    self->cnt++;
    if (self->cnt >= self->max) {
        self->cnt = 0;
    }

        /* 中間データ抽出 */
        /*   中間画素は、"(self->max)/2+1"画素目 */
        /*   配列では、"(self->max)/2"番目となる */
    data = self->List[(self->max)/2].data;

    return (data);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:InitMidData
*機能概要:メディアンフィルタ処理リストデータ初期化
*引数    :CCD_HANDLE        handle i 本オブジェクトアクセスハンドル
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:リスト付ソート処理オブジェクトの初期化を行う。
*備考    :
*
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int InitMidData(CCD_HANDLE handle)
{
    SortSelf *self = (SortSelf *)handle;
    int      i = 0;

        /* パラメータチェック */
    if (self == NULL) {
        return (CCDNG);
    }

    for (i = 0; i < self->max; i++) {
        self->List[i].index = i;
        self->List[i].data = 0;
    }

    return (CCDOK);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:InitMidDatad
*機能概要:メディアンフィルタ処理リストデータ初期化  浮動少数版
*引数    :CCD_HANDLE        handle i 本オブジェクトアクセスハンドル
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:リスト付ソート処理オブジェクトの初期化を行う。
*備考    :
*
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int InitMidDatad(CCD_HANDLE handle)
{
    SortSelfd   *self = (SortSelfd *)handle;
    int         i = 0;

        /* パラメータチェック */
    if (self == NULL) {
        return (CCDNG);
    }

    for (i = 0; i < self->max; i++) {
        self->List[i].index = i;
        self->List[i].data = 0;
    }

    return (CCDOK);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:DeleteGetMidData
*機能概要:メディアンフィルタ処理オブジェクト削除
*引数    :CCD_HANDLE        handle i 本オブジェクトアクセスハンドル
*戻り値  :None
*機能説明:リスト付ソート処理オブジェクトの削除を行う。
*備考    :
*
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

void DeleteGetMidData(CCD_HANDLE handle)
{
    SortSelf *self = (SortSelf *)handle;

        /* 上記を下記に変更 */
    if (self != NULL) {
        if (self->List != NULL) {
/* EditY 2007.03.16
            free(self->List);
*/
		    Mem_Free (self->List);
            self->List = NULL;
        }
/* EditY 2007.03.16
        free(self);
*/
        Mem_Free(self);
        self = NULL;
    }
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:DeleteGetMidDatad
*機能概要:メディアンフィルタ処理オブジェクト削除  浮動少数版
*引数    :CCD_HANDLE        handle i 本オブジェクトアクセスハンドル
*戻り値  :None
*機能説明:リスト付ソート処理オブジェクトの削除を行う。
*備考    :
*
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

void DeleteGetMidDatad(CCD_HANDLE handle)
{
    SortSelfd   *self = (SortSelfd *)handle;

    if (self != NULL) {
        if (self->List != NULL) {
/* EditY 2007.03.16
            free(self->List);
*/
            Mem_Free(self->List);
            self->List = NULL;
        }
/* EditY 2007.03.16
        free(self);
*/
        Mem_Free(self);
        self = NULL;
    }
}

/********************************************************
*       <<CCD data Correction Software Library>>
*関数名称:FilterComp
*機能概要:qsort用の比較関数
*引数    :const void*     a
*         const void*     b
*機能説明:qsort内で使用する
*備考    :
*
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*********************************************************/

int FilterComp(const void* a, const void* b)
{
    if((*(unsigned short int *)a) < (*(unsigned short int *)b)) return (-1);
    if((*(unsigned short int *)a) > (*(unsigned short int *)b)) return (1);
    return (0);
}

/*********************************************************
*       <<CCD data Correction Software Library>>
*関数名称:InsertMidData
*機能概要:中間データ抽出領域へのデータ挿入処理
*引数    :int             As        i   マスクサイズ
*         unsigned short  InData    i   挿入するデータ
*         unsigned short  DelData   i   削除するデータ
*         unsigned short *Din       i/o 中間データ抽出用ソートデータ
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:リスト内にデータを挿入する
*備考    :Dinが昇順にソートされていることが前提
*
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*         update 2006.10.16  YSK 中野   コンパイルワーニング対応(IntelCompiler使用時)
*********************************************************/

int InsertMidData(int As, unsigned short InData, unsigned short DelData, unsigned short *Din)
{
    int            ret = CCDOK;
    int            inPos=0;
    int            first, last;
    int            flg;
    unsigned short tempBuf;
/*    unsigned short *MDin = Din; */
    
        /* パラメータチェック */
    if ((As <= 0) || (Din == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
        /* データ入れ替え */
    first = 0;
    last = As - 1;
    flg = 1;
    while (first <= last) {
        inPos = (first + last) >> 1;
        if (DelData == Din[inPos]) {
            Din[inPos] = InData;
            flg = 0;
            break;
        }
        if (DelData > Din[inPos]) {
            first = inPos + 1;
        } else {
            last = inPos - 1;
        }
    }
    if (flg != 0) {
            /* データ入替え不可だったのでOut! */
        return (CCDNG);
    }

        /* 先頭領域に向かってデータを移動 */
    while ((inPos > 0) && (Din[inPos] < Din[inPos - 1])) {
        tempBuf = Din[inPos - 1];
        Din[inPos - 1] = Din[inPos];
        Din[inPos] = tempBuf;
        inPos -= 1;
    }
        /* 後端領域に向かってデータを移動 */
    last = As - 1;
    while ((inPos < last) && (Din[inPos] > Din[inPos + 1])) {
        tempBuf = Din[inPos + 1];
        Din[inPos + 1] = Din[inPos];
        Din[inPos] = tempBuf;
        inPos += 1;
    }

    return (ret);
}

/*********************************************************
*       <<CCD data Correction Software Library>>
*関数名称:InsertMidDataDouble
*機能概要:中間データ抽出領域へのデータ挿入処理
*引数    :int             As        i   マスクサイズ
*         double		  InData    i   挿入するデータ
*         double		  DelData   i   削除するデータ
*         double		 *Din       i/o 中間データ抽出用ソートデータ
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:リスト内にデータを挿入する
*備考    :Dinが昇順にソートされていることが前提
*
*履歴    :create 2014/8/25	FFS 新井
*********************************************************/

int InsertMidDataDouble(int As, double InData, double DelData, double *Din)
{
    int    ret = CCDOK;
    int    inPos=0;
    int    first, last;
    int    flg;
    double tempBuf;
    
        /* パラメータチェック */
    if ((As <= 0) || (Din == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
        /* データ入れ替え */
    first = 0;
    last = As - 1;
    flg = 1;
    while (first <= last) {
        inPos = (first + last) >> 1;
        if (DelData == Din[inPos]) {
            Din[inPos] = InData;
            flg = 0;
            break;
        }
        if (DelData > Din[inPos]) {
            first = inPos + 1;
        } else {
            last = inPos - 1;
        }
    }
    if (flg != 0) {
            /* データ入替え不可だったのでOut! */
        return (CCDNG);
    }

        /* 先頭領域に向かってデータを移動 */
    while ((inPos > 0) && (Din[inPos] < Din[inPos - 1])) {
        tempBuf = Din[inPos - 1];
        Din[inPos - 1] = Din[inPos];
        Din[inPos] = tempBuf;
        inPos -= 1;
    }
        /* 後端領域に向かってデータを移動 */
    last = As - 1;
    while ((inPos < last) && (Din[inPos] > Din[inPos + 1])) {
        tempBuf = Din[inPos + 1];
        Din[inPos + 1] = Din[inPos];
        Din[inPos] = tempBuf;
        inPos += 1;
    }

    return (ret);
}

/*********************************************************
*       <<CCD data Correction Software Library>>
*関数名称:InsertMidDataLong
*機能概要:中間データ抽出領域へのデータ挿入処理 Longタイプ
*引数    :int             As        i   マスクサイズ
*         long            InData    i   挿入するデータ
*         long            DelData   i   削除するデータ
*         long            *Din      i/o 中間データ抽出用ソートデータ
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:リスト内にデータを挿入する
*備考    :Dinが昇順にソートされていることが前提
*
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*         update 2006.10.16  YSK 中野   コンパイルワーニング対応(IntelCompiler使用時)
*********************************************************/

int InsertMidDataLong(int As, long InData, long DelData, long *Din)
{
    int         ret = CCDOK;
    int         inPos = 0;
    int         first, last;
    int         flg;
    long        tempBuf;
/*    long        *MDin = Din; */
    
        /* パラメータチェック */
    if ((As <= 0) || (Din == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
        /* データ入れ替え */
    first = 0;
    last = As - 1;
    flg = 1;
    while (first <= last) {
        inPos = (first + last) >> 1;
        if (DelData == Din[inPos]) {
            Din[inPos] = InData;
            flg = 0;
            break;
        }
        if (DelData > Din[inPos]) {
            first = inPos + 1;
        } else {
            last = inPos - 1;
        }
    }
    if (flg != 0) {
            /* データ入替え不可だったのでOut! */
        return (CCDNG);
    }

        /* 先頭領域に向かってデータを移動 */
    while ((inPos > 0) && (Din[inPos] < Din[inPos - 1])) {
        tempBuf = Din[inPos - 1];
        Din[inPos - 1] = Din[inPos];
        Din[inPos] = tempBuf;
        inPos -= 1;
    }
        /* 後端領域に向かってデータを移動 */
    last = As - 1;
    while ((inPos < last) && (Din[inPos] > Din[inPos + 1])) {
        tempBuf = Din[inPos + 1];
        Din[inPos + 1] = Din[inPos];
        Din[inPos] = tempBuf;
        inPos += 1;
    }

    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterX
*機能概要:メディアンフィルタ処理 (X方向)
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         unsigned short *Din    i 入力画像         Din(x)
*         unsigned short *Dout   o 出力データ       Dout(x)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:主走査方向のメディアンフィルター処理を行なう。
*           "As=1"は変化がないが無駄に処理時間だけかかる結果になる。
*           その為、"As=1"の場合は、コールしない事を勧める。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*				 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

int MedianFilterX(int xsiz, int ysiz, int As, unsigned short *Din, unsigned short *Dout)
{
    int            ret = CCDOK;
    int            x, y, i, half;
    int            xext;            /* 端延長サイズ */
    int            exsiz;           /* 端延長サイズを加味した画像サイズ */
    unsigned short *Sort = NULL;    /* ソート用バッファ */
    unsigned short *MSort = NULL;
    unsigned short *Data1 = NULL;   /* 端延長画像 */
    unsigned short *MData1 = NULL;
    unsigned short *tempBuf1 = NULL;
    unsigned short *MDin = Din;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
        /* 延長サイズ(マスクサイズの1/2)を算出(切り捨て) */
    xext = As >> 1;
        
        /* 延長サイズを加味した画像サイズ */
    exsiz = xsiz + (xext << 1);
    
        /* メディアン値の抽出位置 */
    half = As >> 1;
    
        /* バッファ確保 */
/* EditY 2007.03.16-ST */
/*
    Sort = (unsigned short *)malloc(As * sizeof(unsigned short));
    Data1 = (unsigned short *)malloc(exsiz * ysiz * sizeof(unsigned short));
*/
    Sort = (unsigned short *)Mem_Alloc(As * sizeof(unsigned short));
    Data1 = (unsigned short *)Mem_Alloc(exsiz * ysiz * sizeof(unsigned short));
/* EditY 2007.03.16-ED */
        
    if ((Sort != NULL) || (Data1 != NULL)) {
        MSort = Sort;
        MData1 = Data1;

            /* 平均延長 */
        ret = AverageExpand(xsiz, ysiz, xext, 0, MDin, MData1);
        if (ret != CCDOK) {
/* EditY 2007.03.16
            free (MData1);
            free (MSort);
*/
            Mem_Free (MData1);
            Mem_Free (MSort);
            return (ret);
        }

        for (y = ysiz; y > 0; y--) {
            Data1 += (As - 1);

            Sort = MSort;
            tempBuf1 = Data1;
            for (i = As; i > 0; i--) {
                (*Sort++) = (*tempBuf1--);
            }
                /* ソート */
            qsort(MSort, As, sizeof(unsigned short), FilterComp);
            
                /* 左端のデータ抽出 */
            (*Dout++) = MSort[half];
            Data1++;

                /* 左端から２画素目以降の処理 */
            for (x = xsiz - 1; x > 0; x--) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidData(As, *Data1, *(Data1 - As), MSort);
/* EditY 2007.03.16
                if (ret != CCDOK) {
                    free (MSort);
                    free (MData1);
                }
*/
                if (ret != CCDOK) {
                    Mem_Free (MSort);
                    Mem_Free (MData1);
                }
					/* データ抽出 */
                (*Dout++) = MSort[half];
                Data1++;
            }
        }
    } else {
            /* バッファ確保失敗 */
        ret = CCDNG;
    }
    
/* EditY 2007.03.16
    if (MSort != NULL) {
        free (MSort);
    }
    if (MData1 != NULL) {
        free (MData1);
    }
*/    
    if (MSort != NULL) {
        Mem_Free (MSort);
    }
    if (MData1 != NULL) {
        Mem_Free (MData1);
    }
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterXd
*機能概要:メディアンフィルタ処理 (X方向) 浮動少数版
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         unsigned short *Din    i 入力画像         Din(x)
*         unsigned short *Dout   o 出力データ       Dout(x)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:主走査方向のメディアンフィルター処理を行なう。
*           "As=1"は変化がないが無駄に処理時間だけかかる結果になる。
*           その為、"As=1"の場合は、コールしない事を勧める。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int MedianFilterXd(int xsiz, int ysiz, int As, double *Din, double *Dout)
{
    int             x, y;
    double          *MemDin = Din;
    double          *MemDout = Dout;
    int             ret = CCDNG;
    CCD_HANDLE      handle = NULL;
    double          data = 0;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        ret = CCDNG_INVALID_PARAM;
        return (ret);
    }
                                /* メディアンフィルタオブジェクト生成 */
    handle = CreateGetMidDatad(As);

    if (handle != NULL){
                                /*------------------------*/
                                /* メディアンフィルタ処理 */
                                /*------------------------*/
        for (y = 0; y < ysiz; y++) {
            Din = MemDin;
            Dout = MemDout;

                                /* メディアンフィルタオブジェクト初期化 */
            InitMidDatad(handle);
                                /*----------------------------------------------*/
                                /* メディアンフィルタ処理前データ編集           */
                                /*----------------------------------------------*/
                                /* 拡張する左端データ生成                       */
            if (MEDIAN_CHG_COFF < As) {
                data = 0;
                for (x = 0; x < (As / 2 + 1); x++) {
                    data += (*Din);
                    Din++;
                }
                Din = MemDin;
                data /= (As / 2) + 1;
            } else {
                data = (*Din);
            }
            for (x = 0; x < (As / 2); x++) {
                                /* 左端部分データをソートリストへ追加               */
                GetMidDatad(handle, data);
            }
            for (x = 0; x < (As / 2); x++) {
                                /* データ上端部(As/2)分データをソートリストへ追加   */
                GetMidDatad(handle, (*Din));
                Din++;
            }

            for (x = 0; x < (xsiz - (As / 2)); x++) {
                                /* メディアンフィルタ処理結果取得                   */
                (*Dout) = GetMidDatad(handle, (*Din));
                Dout++;
                Din++;
            }

                                /* 拡張する右端データ生成                           */
            if (MEDIAN_CHG_COFF < As) {
                Din = MemDin + xsiz - (As / 2 + 1);
                data = 0;
                for (x = 0; x < (As / 2 + 1); x++) {
                    data += (*Din);
                    Din++;
                }
                data /= (As / 2) + 1;
            } else {
                Din = MemDin + xsiz - 1;
                data = (*Din);
            }

            for (x = 0; x < (As / 2); x++) {
                                /* メディアンフィルタ処理結果取得                   */
                (*Dout) = GetMidDatad(handle, data);
                Dout++;
                Din++;
            }
            MemDin += xsiz;
            MemDout += xsiz;
        }
        ret = CCDOK;
    }

    DeleteGetMidDatad(handle);
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterXdVariable
*機能概要:メディアンフィルタ処理 (X方向) 浮動少数版フィルタサイズ可変長
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         double		 *Din    i 入力画像         Din(x)
*         double		 *Dout   o 出力データ       Dout(x)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:主走査方向のメディアンフィルター処理を行なう。
*           "As=1"は変化がないが無駄に処理時間だけかかる結果になる。
*           その為、"As=1"の場合は、コールしない事を勧める。
*備考    :
*履歴    :create 2014.08.01  FFS 新井
*=======================================================*/

int MedianFilterXdVariable(int xsiz, int ysiz, int As, double *Din, double *Dout)
{
    int    ret = CCDOK;
    int    x, y, i, half, cnt;
    long   size;
    double *Sort = NULL;    /* ソート用バッファ */
    double *MSort = NULL;
    double *Data1 = Din;
    double *Data2 = Din;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
    
        /* メディアン値の抽出位置 */
    half = As >> 1;
    cnt = xsiz - (half * 2 + 1);
        /* バッファ確保 */
    size = As * sizeof(double);
    Sort = (double *)Mem_Alloc(size);
  
    MSort = Sort;
    if (Sort != NULL) {
        for (y = ysiz; y > 0; y--) {
            for (i = 0; i < As; i++) {
                (*MSort++) = 0;
            }
            MSort = Sort;
            Data2 = Din;
                /********************/
                /* 左部分処理       */
                /********************/
            for (x = 1; x <= half; x++) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataDouble(As, (*Data1), 0, Sort);
                if (ret != CCDOK) {
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[As-x];
                Dout++;
                Data1++;
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataDouble(As, (*Data1), 0, Sort);
                if (ret != CCDOK) {
                    Mem_Free (MSort);
                    return (ret);
                }
                Data1++;
            }
                /* リストに新しいデータを挿入 */
            ret = InsertMidDataDouble(As, (*Data1), 0, Sort);
            if (ret != CCDOK) {
                Mem_Free (MSort);
                return (ret);
            }
            (*Dout) = Sort[half];
            Dout++;
            Data1++;

                /********************/
                /* 中央部分処理     */
                /********************/
            for (x = cnt; x > 0; x--) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataDouble(As, (*Data1), (*Data2), Sort);
                if (ret != CCDOK) {
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half];
                Dout++;
                Data1++;
                Data2++;
            }

                /********************/
                /* 右部分処理       */
                /********************/
            for (x = 1; x <= half; x++) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataDouble(As, 0, (*Data2), Sort);
                if (ret != CCDOK) {
                    Mem_Free (MSort);
                    return (ret);
                }
                Data2++;
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataDouble(As, 0, (*Data2), Sort);
                if (ret != CCDOK) {
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half+x];
                Dout++;
                Data2++;
            }
            Din += xsiz;
        }
    } else {
            /* バッファ確保失敗 */
        ret = CCDNG;
    }
    
    if (MSort != NULL) {
        Mem_Free (MSort);
    }
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterY
*機能概要:メディアンフィルタ処理 (Y方向)
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         unsigned short *Din    i 入力画像         Din(x,y)
*         unsigned short *Dout   o 出力データ       Dout(x,y)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:副走査方向のメディアンフィルター処理を行なう。
*           "As=1"は変化がないが無駄に処理時間だけかかる結果になる。
*           その為、"As=1"の場合は、コールしない事を勧める。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*		  update 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

int MedianFilterY(int xsiz, int ysiz, int As, unsigned short *Din, unsigned short *Dout)
{
    int            ret = CCDOK;
    int            x, y, i, half, ypos;
    int            num, extnum;
    int            yext;            /* 端延長サイズ */
    int            eysiz;           /* 端延長サイズを加味した画像サイズ */
    unsigned short *Sort = NULL;    /* ソート用バッファ */
    unsigned short *MSort = NULL;
    unsigned short *Sort1 = NULL;
    unsigned short *MSort1 = NULL;
    unsigned short *Data1 = NULL;   /* 端延長画像 */
    unsigned short *MData1 = NULL;
    unsigned short *tempBuf1 = NULL;
    unsigned short *MDin = Din;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
        /* 延長サイズ(マスクサイズの1/2)を算出(切り捨て) */
    yext = As >> 1;
        
        /* 延長サイズを加味した画像サイズ */
    eysiz = ysiz + (yext << 1);
    
        /* メディアン値の抽出位置 */
    half = As >> 1;
    
    ypos = As * xsiz;
    num = xsiz * ysiz;
    extnum = xsiz * eysiz;
    
        /* バッファ確保 */
/* EditY 2007.03.16-ST */
/*
    Sort = (unsigned short *)malloc(As * sizeof(unsigned short));
    Sort1 = (unsigned short *)malloc(As * xsiz * sizeof(unsigned short));
    Data1 = (unsigned short *)malloc(eysiz * xsiz * sizeof(unsigned short));
*/
    Sort = (unsigned short *)Mem_Alloc(As * sizeof(unsigned short));
    Sort1 = (unsigned short *)Mem_Alloc(As * xsiz * sizeof(unsigned short));
    Data1 = (unsigned short *)Mem_Alloc(eysiz * xsiz * sizeof(unsigned short));
/* EditY 2007.03.16-ED */
        
    if ((Sort != NULL) || (Sort1 != NULL) || (Data1 != NULL)) {
        MSort = Sort;
        MData1 = Data1;
        MSort1 = Sort1;

            /* 平均延長 */
        ret = AverageExpand(xsiz, ysiz, 0, yext, MDin, MData1);
        if (ret != CCDOK) {
/* EditY 2007.03.16
            free (MData1);
            free (MSort);
            free (MSort1);
*/
            Mem_Free (MData1);
            Mem_Free (MSort);
            Mem_Free (MSort1);
            return (ret);
        }

        Data1 += (As - 1) * xsiz;
            /* 上端データの作成 */
        for (x = xsiz; x > 0; x--) {
            tempBuf1 = Data1;
            Sort = MSort;
            for (i = As; i > 0; i--) {
                (*Sort++) = (*tempBuf1);
                tempBuf1 -= xsiz;
            }
                /* ソート */
            qsort(MSort, As, sizeof(unsigned short), FilterComp);
            
                /* データ抽出 */
            (*Dout++) = MSort[half];
            
                /* ソートデータの退避 */
            Sort = MSort;
            for (i = As; i > 0; i--) {
                (*Sort1++) = (*Sort++);
            }
            Data1++;
        }

            /* ２ライン目以降の処理 */
        for (y = ysiz - 1; y > 0; y--) {
            Sort1 = MSort1;
            for (x = 0; x < xsiz; x++) {
                ret = InsertMidData(As, (*Data1), (*(Data1 - ypos)), Sort1);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
                    free (MData1);
                    free (MSort1);
*/
                    Mem_Free (MSort);
                    Mem_Free (MData1);
                    Mem_Free (MSort1);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = MSort1[(x * As) + half];
                Dout++;
                Data1++;
                Sort1 += As;
            }
        }
    } else {
            /* バッファ確保失敗 */
        ret = CCDNG;
    }
    
/* EditY 2007.03.16
    if (MSort != NULL) {
        free (MSort);
    }
    if (MData1 != NULL) {
        free (MData1);
    }
    if (MSort1 != NULL) {
        free (MSort1);
    }
*/    
    if (MSort != NULL) {
        Mem_Free (MSort);
    }
    if (MData1 != NULL) {
        Mem_Free (MData1);
    }
    if (MSort1 != NULL) {
        Mem_Free (MSort1);
    }
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterYd
*機能概要:メディアンフィルタ処理 (Y方向) 浮動少数版
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         unsigned short *Din    i 入力画像         Din(x,y)
*         unsigned short *Dout   o 出力データ       Dout(x,y)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:副走査方向のメディアンフィルター処理を行なう。
*           "As=1"は変化がないが無駄に処理時間だけかかる結果になる。
*           その為、"As=1"の場合は、コールしない事を勧める。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*=======================================================*/

int MedianFilterYd(int xsiz, int ysiz, int As, double *Din, double *Dout)
{
    int             x, y;
    double          *MemDin = Din;
    double          *MemDout = Dout;
    int             ret = CCDNG;
    CCD_HANDLE      handle = NULL;
    double          data = 0;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        ret = CCDNG_INVALID_PARAM;
        return (ret);
    }

                                /* メディアンフィルタオブジェクト生成 */
    handle = CreateGetMidDatad(As);

    if (handle != NULL) {
                                /*------------------------*/
                                /* メディアンフィルタ処理 */
                                /*------------------------*/
        for (x = 0; x < xsiz; x++) {
            Din = MemDin;
            Dout = MemDout;

                                /* メディアンフィルタオブジェクト初期化 */
            InitMidDatad(handle);

                                /*----------------------------------------------*/
                                /* メディアンフィルタ処理前データ編集           */
                                /*----------------------------------------------*/
                                /* 拡張する左端データ生成                       */
            if (MEDIAN_CHG_COFF < As) {
                data = 0;
                for (y = 0; y < (As / 2 + 1); y++) {
                                /* 上端データ生成                               */
                    data += (*Din);
                    Din += xsiz;
                }
                Din = MemDin;
                data /= (As / 2) + 1;
            } else {
                data = (*Din);
            }

            for (y = 0; y < (As / 2); y++) {
                                /* 上端部分データをソートリストへ追加           */
                GetMidDatad(handle, data);
            }

            for (y = 0; y < (As / 2); y++) {
                                /* メディアンフィルタ処理結果取得               */
                GetMidDatad(handle, *Din);
                Din += xsiz;
            }

            for (y = 0; y < (ysiz - (As / 2)); y++) {
                                /* メディアンフィルタ処理結果取得               */
                *Dout = GetMidDatad(handle, *Din);
                if (*Dout != 0) {
                    xsiz = xsiz;
                }
                Dout += xsiz;
                Din += xsiz;
            }

                                /* 拡張する左端データ生成                       */
            if (MEDIAN_CHG_COFF < As) {
                Din = MemDin + (xsiz * (ysiz - ((As / 2) + 1)));
                data = 0;
                for (y = 0; y < ((As / 2) + 1); y++) {
                    data += (*Din);
                    Din += xsiz;
                }
                data /= (As / 2) + 1;
            } else {
                Din = MemDin + (xsiz * (ysiz - 1));
                data = (*Din);
            }
            for (y = 0; y < (As /2); y++) {
                                /* メディアンフィルタ処理結果取得               */
                *Dout = GetMidDatad(handle, data);
                Dout += xsiz;
                Din += xsiz;
            }
            MemDin++;
            MemDout++;
        }
        ret = CCDOK;
    }

    DeleteGetMidDatad(handle);
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterXY
*機能概要:メディアンフィルタ処理 (消去残像補正データ生成用減衰率テーブル作成用)
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             AsX    i メディアンフィルタマスクサイズＸ
*         int             AsY    i メディアンフィルタマスクサイズＹ
*         unsigned short *Din    i 入力画像         Din(x,y)
*         unsigned short *Dout   i 出力画像         Dout(x,y)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG_INVALID_PARAM)
*機能説明:主＆副走査方向のメディアンフィルター処理を行なう。
*           "As=1"は変化がないが無駄に処理時間だけかかる結果になる。
*           その為、"As=1"の場合は、コールしない事を勧める。
*備考    :
*履歴    :create 2006.07.26  YSK 宮崎   FPD1化
*         update 2006.10.16  YSK 中野   コンパイルワーニング対応(IntelCompiler使用時)
*				 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

int MedianFilterXY(int xsiz, int ysiz, int AsX, int AsY, unsigned short *Din, unsigned short *Dout)
{
/*    int             x = 0, y = 0, i = 0; */
	int             y = 0;
    int             ret = CCDNG;
    int             size = 0;
    unsigned short  *MDin = Din;
    unsigned short  *Current = NULL;
    unsigned short  *MCurrent = NULL;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (AsX <= 0) || (AsY <= 0) || (Din == NULL) || (Dout == NULL)) {
        ret = CCDNG_INVALID_PARAM;
        return (ret);
    }

    size = xsiz * sizeof(unsigned short);
/* EditY 2007.03.16-ST */
/*
    Current = (unsigned short *)malloc(size);
*/
    Current = (unsigned short *)Mem_Alloc(size);
/* EditY 2007.03.16-ED */
    if (Current != NULL) {
        ret = CCDOK;
        MCurrent = Current;

        for (y = 0; y < ysiz; y++) {
            memcpy(Current, Din, size);
            MedianFilterX(xsiz, 1, AsX, Current, Din);
            Din += xsiz;
        }

        MedianFilterY(xsiz, ysiz, AsY, MDin, Dout);
    }

/* EditY 2007.03.16
    if (MCurrent != NULL) {
        free (MCurrent);
    }
*/
    if (MCurrent != NULL) {
        Mem_Free (MCurrent);
    }
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterX2
*機能概要:メディアンフィルタ処理 (X方向)
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         unsigned short *Din    i 入力画像         Din(x)
*         unsigned short *Dout   o 出力データ       Dout(x)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:主走査方向のメディアンフィルター処理を行なう。
*備考    :入出力バッファは別なメモリ空間にする事
*           同バッファを指定すると、"CCDNG"が発生する。
*履歴    :create 2006.09.13  YSK 宮崎   端処理変更
*         update 2006.09.20  YSK 宮崎   不具合修正
*				 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

int MedianFilterX2(int xsiz, int ysiz, int As, unsigned short *Din, unsigned short *Dout)
{
    int            ret = CCDOK;
    int            x, y, i, half, cnt;
    long           size;
    unsigned short *Sort = NULL;    /* ソート用バッファ */
    unsigned short *MSort = NULL;
    unsigned short *Data1 = Din;
    unsigned short *Data2 = Din;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
    
        /* メディアン値の抽出位置 */
    half = As >> 1;
    cnt = xsiz - (half * 2 + 1);
        /* バッファ確保 */
    size = As * sizeof(unsigned short);
/* EditY 2007.03.16-ST */
/*
    Sort = (unsigned short *)malloc(size);
*/
    Sort = (unsigned short *)Mem_Alloc(size);
/* EditY 2007.03.16-ED */
    MSort = Sort;
    if (Sort != NULL) {
        for (y = ysiz; y > 0; y--) {
            for (i = 0; i < As; i++) {
                (*MSort++) = 0;
            }
            MSort = Sort;
            Data2 = Din;
                /********************/
                /* 左部分処理       */
                /********************/
            for (x = 1; x <= half; x++) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidData(As, (*Data1), 0, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[As-x];
                Dout++;
                Data1++;
                    /* リストに新しいデータを挿入 */
                ret = InsertMidData(As, (*Data1), 0, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Data1++;
            }
                /* リストに新しいデータを挿入 */
            ret = InsertMidData(As, (*Data1), 0, Sort);
            if (ret != CCDOK) {
/* EditY 2007.03.16
                free (MSort);
*/
                Mem_Free (MSort);
                return (ret);
            }
            (*Dout) = Sort[half];
            Dout++;
            Data1++;

                /********************/
                /* 中央部分処理     */
                /********************/
            for (x = cnt; x > 0; x--) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidData(As, (*Data1), (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half];
                Dout++;
                Data1++;
                Data2++;
            }

                /********************/
                /* 右部分処理       */
                /********************/
            for (x = 1; x <= half; x++) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidData(As, 0, (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Data2++;
                    /* リストに新しいデータを挿入 */
                ret = InsertMidData(As, 0, (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half+x];
                Dout++;
                Data2++;
            }
            Din += xsiz;
        }
    } else {
            /* バッファ確保失敗 */
        ret = CCDNG;
    }
    
/* EditY 2007.03.16
    if (MSort != NULL) {
        free (MSort);
    }
*/    
    if (MSort != NULL) {
        Mem_Free (MSort);
    }
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterY2
*機能概要:メディアンフィルタ処理 (Y方向)
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         unsigned short *Din    i 入力画像         Din(x,y)
*         unsigned short *Dout   o 出力データ       Dout(x,y)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:副走査方向のメディアンフィルター処理を行なう。
*         端処理が、存在すする画素数にてメディアン実施
*               ((As/2)+1)の平均延長ではない。
*備考    :入出力バッファは別なメモリ空間にする事
*           同バッファを指定すると、"CCDNG"が発生する。
*履歴    :create 2006.08.21  YSK 宮崎   端処理別タイプ作成
*         update 2006.09.20  YSK 宮崎   不具合修正
*				 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

int MedianFilterY2( int xsiz, int ysiz, int As, unsigned short *Din, unsigned short *Dout)
{
    int             ret = CCDOK;
    int             x, y, i, half, cnt, max;
    long            size;
    unsigned short *Sort = NULL;    /* ソート用バッファ */
    unsigned short *MSort = NULL;
    unsigned short *Data1 = Din;
    unsigned short *Data2 = Din;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }

        /* メディアン値の抽出位置 */
    half = As >> 1;
    cnt = ysiz - (half * 2 + 1);
    max = As * xsiz;
    size = max * sizeof(unsigned short);

        /* バッファ確保 */
/* EditY 2007.03.16-ST */
/*
    Sort = (unsigned short *)malloc(size);
*/
    Sort = (unsigned short *)Mem_Alloc(size);
/* EditY 2007.03.16-ED */
    if (Sort != NULL) {
        MSort = Sort;
            /* ソートバッファ初期化 */
        for (i = 0; i < max; i++) {
            (*MSort++) = 0;
        }
        MSort = Sort;

            /********************/
            /* 上部分の処理     */
            /********************/
        Data2 = Data1 + xsiz;
        for (y = 1; y <= half; y++) {
            Sort = MSort;
            for (x = 0; x < xsiz; x++) {
                ret = InsertMidData(As, (*Data1), 0, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[As-y];

                ret = InsertMidData(As, (*(Data2)), 0, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Dout++;
                Data1++;
                Data2++;
                Sort += As;
            }
            Data1 += xsiz;
            Data2 += xsiz;
        }
        Sort = MSort;
        for (x = 0; x < xsiz; x++) {
            ret = InsertMidData(As, (*Data1), 0, Sort);
            if (ret != CCDOK) {
/* EditY 2007.03.16
                free (MSort);
*/
                Mem_Free (MSort);
                return (ret);
            }
                /* データ抽出 */
            (*Dout) = Sort[half];
            Dout++;
            Data1++;
            Sort += As;
        }

            /********************/
            /* 中央部分の処理   */
            /********************/
        Data2 = Din;
        for (y = cnt; y > 0; y--) {
            Sort = MSort;
            for (x = 0; x < xsiz; x++) {
                ret = InsertMidData(As, (*Data1), (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half];
                Dout++;
                Data1++;
                Data2++;
                Sort += As;
            }
        }

            /********************/
            /* 下部分の処理     */
            /********************/
        Data1 = Data2 + xsiz;
        for (y = 1; y <= half; y++) {
            Sort = MSort;
            for (x = 0; x < xsiz; x++) {
                ret = InsertMidData(As, 0, (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Data2++;
                ret = InsertMidData(As, 0, (*Data1), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half + y];
                Dout++;
                Data1++;
                Sort += As;
            }
            Data1 += xsiz;
            Data2 += xsiz;
        }
    } else {
            /* バッファ確保失敗 */
        ret = CCDNG;
    }
    
/* EditY 2007.03.16
    if (MSort != NULL) {
        free (MSort);
    }
*/    
    if (MSort != NULL) {
        Mem_Free (MSort);
    }
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterX2Long
*機能概要:メディアンフィルタ処理 (X方向) Longタイプ
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         long            *Din   i 入力画像         Din(x)
*         long            *Dout  o 出力データ       Dout(x)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:主走査方向のメディアンフィルター処理を行なう。
*備考    :入出力バッファは別なメモリ空間にする事
*           同バッファを指定すると、"CCDNG"が発生する。
*履歴    :create 2006.09.25  YSK 宮崎   バッファサイズカスタマイズ
*		  update 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

int MedianFilterX2Long(int xsiz, int ysiz, int As, long *Din, long *Dout)
{
    int             ret = CCDOK;
    int             x, y, i, half, cnt;
    long            size;
    long            *Sort = NULL;    /* ソート用バッファ */
    long            *MSort = NULL;
    long            *Data1 = Din;
    long            *Data2 = Din;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }
    
        /* メディアン値の抽出位置 */
    half = As >> 1;
    cnt = xsiz - (half * 2 + 1);
        /* バッファ確保 */
    size = As * sizeof(long);
/* EditY 2007.03.16-ST */
/*
    Sort = (long *)malloc(size);
*/
    Sort = (long *)Mem_Alloc(size);
/* EditY 2007.03.16-ED */
    MSort = Sort;
    if (Sort != NULL) {
        for (y = ysiz; y > 0; y--) {
//            for (i = 0; i < As; i++) {
            for (i = As; i > 0; i--) {
                (*MSort++) = (long)NEW_MEDIAN_DEFAULT;
            }
            MSort = Sort;
            Data2 = Din;
                /********************/
                /* 左部分処理       */
                /********************/
            for (x = 1; x <= half; x++) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataLong(As, (*Data1), NEW_MEDIAN_DEFAULT, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[As-x];
                Dout++;
                Data1++;
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataLong(As, (*Data1), NEW_MEDIAN_DEFAULT, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Data1++;
            }
                /* リストに新しいデータを挿入 */
            ret = InsertMidDataLong(As, (*Data1), NEW_MEDIAN_DEFAULT, Sort);
            if (ret != CCDOK) {
/* EditY 2007.03.16
                free (MSort);
*/
                Mem_Free (MSort);
                return (ret);
            }
            (*Dout) = Sort[half];
            Dout++;
            Data1++;

                /********************/
                /* 中央部分処理     */
                /********************/
            for (x = cnt; x > 0; x--) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataLong(As, (*Data1), (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half];
                Dout++;
                Data1++;
                Data2++;
            }

                /********************/
                /* 右部分処理       */
                /********************/
            for (x = 1; x <= half; x++) {
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataLong(As, NEW_MEDIAN_DEFAULT, (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Data2++;
                    /* リストに新しいデータを挿入 */
                ret = InsertMidDataLong(As, NEW_MEDIAN_DEFAULT, (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half+x];
                Dout++;
                Data2++;
            }
            Din += xsiz;
        }
    } else {
            /* バッファ確保失敗 */
        ret = CCDNG;
    }
    
/* EditY 2007.03.16
    if (MSort != NULL) {
        free (MSort);
    }
*/    
    if (MSort != NULL) {
        Mem_Free (MSort);
    }
    return (ret);
}

/*=======================================================
*       <<CCD data Correction Software Library>>
*関数名称:MedianFilterY2Long
*機能概要:メディアンフィルタ処理 (Y方向) Longタイプ
*引数    :int             xsiz   i 主方向画素数
*         int             ysiz   i 副方向画素数
*         int             As     i メディアンフィルタマスクサイズ
*         long            *Din   i 入力画像         Din(x,y)
*         long            *Dout  o 出力データ       Dout(x,y)
*戻り値  :int   正常 (CCDOK)
*               異常 (CCDNG)
*                    (CCDNG_INVALID_PARAM)
*機能説明:副走査方向のメディアンフィルター処理を行なう。
*         端処理が、存在すする画素数にてメディアン実施
*               ((As/2)+1)の平均延長ではない。
*備考    :入出力バッファは別なメモリ空間にする事
*           同バッファを指定すると、"CCDNG"が発生する。
*履歴    :create 2006.09.25  YSK 宮崎   バッファサイズカスタマイズ
*		  update 2007.03.16	 YSK 橋口   メモリ管理処理の追加による対応
*=======================================================*/

int MedianFilterY2Long(int xsiz, int ysiz, int As, long *Din, long *Dout)
{
    int             ret = CCDOK;
    int             x, y, i, half, cnt, max;
    long            size;
    long            *Sort = NULL;    /* ソート用バッファ */
    long            *MSort = NULL;
    long            *Data1 = Din;
    long            *Data2 = Din;

        /* パラメータチェック */
    if ((xsiz <= 0) || (ysiz <= 0) || (As <= 0) || (Din == NULL) || (Dout == NULL)) {
        return (CCDNG_INVALID_PARAM);
    }

        /* メディアン値の抽出位置 */
    half = As >> 1;
    cnt = ysiz - (half * 2 + 1);
    max = As * xsiz;
    size = max * sizeof(long);

        /* バッファ確保 */
/* EditY 2007.03.16-ST */
/*
    Sort = (long *)malloc(size);
*/
    Sort = (long *)Mem_Alloc(size);
/* EditY 2007.03.16-ED */
    if (Sort != NULL) {
        MSort = Sort;
            /* ソートバッファ初期化 */
//        for (i = 0; i < max; i++) {
        for (i = max; i > 0; i--) {
            (*MSort++) = (long)NEW_MEDIAN_DEFAULT;
        }
        MSort = Sort;

            /********************/
            /* 上部分の処理     */
            /********************/
        Data2 = Data1 + xsiz;
        for (y = 1; y <= half; y++) {
            Sort = MSort;
//            for (x = 0; x < xsiz; x++) {
            for (x = xsiz; x > 0; x--) {
                ret = InsertMidDataLong(As, (*Data1), NEW_MEDIAN_DEFAULT, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[As-y];

                ret = InsertMidDataLong(As, (*(Data2)), NEW_MEDIAN_DEFAULT, Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Dout++;
                Data1++;
                Data2++;
                Sort += As;
            }
            Data1 += xsiz;
            Data2 += xsiz;
        }
        Sort = MSort;
//        for (x = 0; x < xsiz; x++) {
        for (x = xsiz; x > 0; x--) {
            ret = InsertMidDataLong(As, (*Data1), NEW_MEDIAN_DEFAULT, Sort);
            if (ret != CCDOK) {
/* EditY 2007.03.16
                free (MSort);
*/
                Mem_Free (MSort);
                return (ret);
            }
                /* データ抽出 */
            (*Dout) = Sort[half];
            Dout++;
            Data1++;
            Sort += As;
        }

            /********************/
            /* 中央部分の処理   */
            /********************/
        Data2 = Din;
        for (y = cnt; y > 0; y--) {
            Sort = MSort;
//            for (x = 0; x < xsiz; x++) {
            for (x = xsiz; x > 0; x--) {
                ret = InsertMidDataLong(As, (*Data1), (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half];
                Dout++;
                Data1++;
                Data2++;
                Sort += As;
            }
        }

            /********************/
            /* 下部分の処理     */
            /********************/
        Data1 = Data2 + xsiz;
        for (y = 1; y <= half; y++) {
            Sort = MSort;
//            for (x = 0; x < xsiz; x++) {
            for (x = xsiz; x > 0; x--) {
                ret = InsertMidDataLong(As, NEW_MEDIAN_DEFAULT, (*Data2), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                Data2++;
                ret = InsertMidDataLong(As, NEW_MEDIAN_DEFAULT, (*Data1), Sort);
                if (ret != CCDOK) {
/* EditY 2007.03.16
                    free (MSort);
*/
                    Mem_Free (MSort);
                    return (ret);
                }
                    /* データ抽出 */
                (*Dout) = Sort[half + y];
                Dout++;
                Data1++;
                Sort += As;
            }
            Data1 += xsiz;
            Data2 += xsiz;
        }
    } else {
            /* バッファ確保失敗 */
        ret = CCDNG;
    }
    
/* EditY 2007.03.16
    if (MSort != NULL) {
        free (MSort);
    }
*/    
    if (MSort != NULL) {
        Mem_Free (MSort);
    }
    return (ret);
}

/*** End of Program ***/
