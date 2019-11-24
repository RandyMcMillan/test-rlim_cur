//
//  main.cpp
//  test-rlim_cur
//
//  Created by git on 11/23/19.
//  Copyright © 2019 randymcmillan.net. All rights reserved.
//

#include <iostream>
#include <sys/resource.h>
#include <stdio.h>

int RaiseFileDescriptorLimit(int nMinFD) {
#if defined(WIN32)
    return 2048;
#else
    struct rlimit limitFD;
    if (getrlimit(RLIMIT_NOFILE, &limitFD) != -1) {
        if (limitFD.rlim_cur < (rlim_t)nMinFD) {
            limitFD.rlim_cur = nMinFD;
            if (limitFD.rlim_cur > limitFD.rlim_max)
                limitFD.rlim_cur = limitFD.rlim_max;
            setrlimit(RLIMIT_NOFILE, &limitFD);
            getrlimit(RLIMIT_NOFILE, &limitFD);
        }
        //recasting limitFD.rlim_cur to int from long long unsigned
        return (int)limitFD.rlim_cur;
    }
    return nMinFD; // getrlimit failed, assume it's fine
#endif
}

int main(int argc, const char * argv[]) {

        struct rlimit rl;
        int err;

        err = getrlimit(RLIMIT_STACK, &rl);
        if (err < 0) {
            perror("getrlimit");
            return err;
        }

        printf("%d, soft=%llu, hard=%llu\n", RLIMIT_STACK, rl.rlim_cur, rl.rlim_max);

        err = setrlimit(RLIMIT_STACK, &rl);
        if (err < 0) {
            perror("setrlimit");
            return err;
        }

        printf("%d\n",RaiseFileDescriptorLimit(256));
    
        return 0;
    }
