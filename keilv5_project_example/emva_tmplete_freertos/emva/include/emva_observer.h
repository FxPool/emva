// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EMVA_OB_SERVER_H_
#define __EMVA_OB_SERVER_H_

#include "emva_list.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        void *key;
        void *value;
    } obServerMap;
    typedef struct _subject Subject;
    typedef struct _obServer ObServer;
    struct _subject
    {
        char *Name;
        Link_List *ObMapList;
        void (*Notify)(Subject *s, const char *messges);
        void (*Add)(Subject *s, ObServer *ob);
        void (*Del)(Subject *s, ObServer *ob);
    };

    struct _obServer
    {
        char *Name;
        void (*upDataProc)(ObServer *ob, Subject *s, const char *messges);
    };
    typedef void (*obMsgCallFunc)(ObServer *ob, Subject *s, const char *messges);
    typedef struct
    {
        Subject *(*PublishTopic)(char *topicName);
        uint8 (*SubscribeTopic)(char *obName, const char *topicName, obMsgCallFunc obMsgCall);
    } ObserverPattern_t;
    typedef struct EmvaObserverApi emva_observer;
    struct EmvaObserverApi
    {
        ObserverPattern_t *(*SingletonNew)(void);
    };
    extern emva_observer EmvaObserver;
#ifdef __cplusplus
}
#endif
#endif
