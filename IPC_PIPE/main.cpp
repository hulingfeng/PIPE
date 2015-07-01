//
//  main.cpp
//  IPC_PIPE
//
//  Created by hlf on 15/6/22.
//  Copyright (c) 2015年 hlf. All rights reserved.
//


/*
  
管道   分为 匿名管道  和  命名管道
 
 匿名管道，只适合有亲缘关系的进程之间使用
 
 */
#include <iostream>
#include <unistd.h>

#include <string.h>
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    //std::cout << "Hello, World!\n";
    
    /*匿名管道*/
    //创建匿名管道
    int pipe_fid[2];
    memset(pipe_fid, 0, sizeof(pipe_fid));
    if (pipe(pipe_fid) < 0) {
        cout << "pipe error" << endl;
    }
    char operatorbuf[16] ={0};
    //创建子进程
    pid_t pid = fork();
    if (pid == 0) {
        //子进程(写数据)
        // 关闭读管道
        close(pipe_fid[0]);
        
        //循环写数据
        for (int i = 0 ; i < 20; i++) {
            sprintf(operatorbuf,"%d",i);
            write(pipe_fid[1],operatorbuf,sizeof(operatorbuf));
            sleep(1);
        }
        write(pipe_fid[1],"exit",strlen("exit"));
        cout << "子进程结束" <<endl;
        sleep(5);
        exit(0);
        
    }else if (pid > 0){
        //父进程(读数据)
        //关闭写数据
        close(pipe_fid[1]);
        
        memset(operatorbuf, 0, sizeof(operatorbuf));
        while (read(pipe_fid[0],operatorbuf, sizeof(operatorbuf)) >= 0) {
            cout << "父进程读取的数据:" << operatorbuf <<endl;
            if (strcmp(operatorbuf, "exit") == 0){
                break;
            }
        }
        cout << "等待子进程结束－－start" <<endl;
        wait(NULL);
        cout << "等待子进程结束－－end" <<endl;
    }else{
        //错误
        cout << "fork error" <<endl;
    }
    
    
    
    return 0;
}
