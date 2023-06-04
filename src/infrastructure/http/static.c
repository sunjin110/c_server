#include <dirent.h>
#include "static.h"
// #include <stdlib.h>
#include <sys/param.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "../../common/logger/logger.h"

// TODO 起動時にstaticファイル配下のファイルをロードして、メモリに載せる  
// TODO 対象のpathが帰ってきた時に、それを返す仕組みを作る

// init_static_contents 静的なファイルをメモリに載せる、pathがきた時に返せるようにする
extern void init_static_contents() {
    // TODO やること
    
    // ディレクトリ static配下のディレクトリをwalkして、pathを全て取得する
        // この時、拡張子で取得するかしないかを取捨選択したい
    // pathにあるcontentの内容を取得する
    // globalなhash_mapに追加する
}


extern void walk_test() {

    // open dir
    // DIR *dir = opendir("./static");
    char pwd[1024];
    getcwd(&pwd, sizeof(pwd));
    DEBUG_PRINTLN("PWD is %s\n", pwd);

    // getcwd();

    DIR *dir = opendir("/Users/yun_sunjin/develop/c_server/src/infrastructure/http/static");
    if (dir == NULL) {
        ERR_PRINTLN("failed dir open. errno: %d", errno);
        return;
    }

    struct dirent *ent;
    for (;;) {
        ent = readdir(dir);
        if (ent == NULL) {
            break;
        }
        DEBUG_PRINTLN("%s\n", ent->d_name);
    }

}