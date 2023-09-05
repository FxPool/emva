// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_observer.h"
#include "string.h"
#include "stdio.h"
#include "emva_malloc.h"
#if EMVA_FM_OBSERVER_ENABLED == 1
static EmvaLink_List *gSubjectList = NULL;

static void thisAdd(Subject *s, ObServer *ob)
{
    obServerMap *obMap = (obServerMap *)emva_malloc(sizeof(obServerMap));
    obMap->key = (void *)ob->Name;
    obMap->value = (void *)ob;
    EmvaLink_List_Insert(s->ObMapList, obMap, -1);
}
static void thisDel(Subject *s, ObServer *ob)
{
    object_t object;
    obServerMap obMap;
    obMap.key = (void *)ob->Name;
    obMap.value = (void *)ob;
    object.data = (byte *)&obMap;
    object.length = sizeof(obServerMap);
    EmvaLink_List_object_RemoveAt(s->ObMapList, &object);
}
static void thisNotify(Subject *s, const char *messages)
{
    obServerMap *kv = NULL;
    for (uint8 i = 0; i < s->ObMapList->length; i++)
    {
        kv = (obServerMap *)EmvaLink_List_GetAt(s->ObMapList, i);
        ((ObServer *)kv->value)->upDataProc((ObServer *)kv->value, s, messages);
    }
}
// Create a new theme subscriber
ObServer *obServerNew(const char *obName, obMsgCallFunc obMsgCall)
{
    ObServer *ob = (ObServer *)emva_malloc(sizeof(ObServer));
    ob->Name = (char *)emva_malloc(sizeof(char) * (strlen(obName) + 1));
    memcpy(ob->Name, obName, strlen(obName));
    ob->upDataProc = obMsgCall;
    return ob;
}
// Create a new theme publisher
Subject *subjectNew(const char *topicName)
{
    Subject *s = (Subject *)emva_malloc(sizeof(Subject));
    s->Name = (char *)emva_malloc(sizeof(char) * (strlen(topicName) + 1));
    memcpy(s->Name, topicName, strlen(topicName));
    s->ObMapList = EmvaLink_List_Init();
    s->Notify = thisNotify;
    s->Add = thisAdd;
    s->Del = thisDel;
    return s;
}

// Create stand-alone publish/subscribe logic
Subject *thisPublishTopic(char *topicName)
{
    obServerMap *obMap = NULL;
    for (uint8 i = 0; i < gSubjectList->length; i++)
    {
        obMap = (obServerMap *)EmvaLink_List_GetAt(gSubjectList, i);
        if (strcmp((char *)obMap->key, topicName) == 0)
        {
            return NULL;
        }
    }
    obMap = (obServerMap *)emva_malloc(sizeof(obServerMap));
    obMap->key = topicName;
    obMap->value = subjectNew(topicName);
    EmvaLink_List_Insert(gSubjectList, obMap, -1);

    return obMap->value;
}
uint8 thisSubscribeTopic(char *obName, const char *topicName, obMsgCallFunc obMsgCall)
{
    obServerMap *obMap = NULL;
    for (uint8 i = 0; i < gSubjectList->length; i++)
    {
        obMap = (obServerMap *)EmvaLink_List_GetAt(gSubjectList, i);
        if (strcmp((char *)obMap->key, topicName) == 0)
        {
            Subject *s = (Subject *)obMap->value;
            // Check if the subscriber's name is the same
            for (uint8 i = 0; i < s->ObMapList->length; i++)
            {
                obServerMap *obMap = (obServerMap *)EmvaLink_List_GetAt(s->ObMapList, i);
                if (strcmp((char *)obMap->key, obName) == 0)
                {
                    return 0;
                }
            }
            s->Add(s, obServerNew(obName, obMsgCall));
            return 1;
        }
    }
    return 0;
}
// Singleton mode
static ObserverPattern_t *ObserverSingletonNew(void)
{
    static ObserverPattern_t ObserverPattern;
    ObserverPattern.PublishTopic = thisPublishTopic;
    ObserverPattern.SubscribeTopic = thisSubscribeTopic;
    if (gSubjectList == NULL)
    {
        gSubjectList = EmvaLink_List_Init();
    }
    return &ObserverPattern;
}

// api
emva_observer EmvaObserver = {
    .SingletonNew = ObserverSingletonNew,
};
#endif
