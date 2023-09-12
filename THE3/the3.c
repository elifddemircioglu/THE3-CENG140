
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "the3.h"



void refreshlist(DomainFreqNode *ndfn, CacheNode *pcn){
    
    DomainFreqNode *cdfn, *pdfn ,*ptr;
    int listindex = 0;

    ptr = pcn->domainFreqList->head;
    while(ptr){
        ptr = ptr->next;
        listindex++;
    }

    if(listindex==0){
        pcn->domainFreqList->head = ndfn;
        pcn->domainFreqList->tail = ndfn;
        ndfn -> next = NULL;
        ndfn -> prev = NULL;
        pdfn = ndfn;
    }    
    else if(listindex==1){

        if((pcn->domainFreqList->head)->freq < ndfn->freq || (pcn->domainFreqList->head->freq == ndfn->freq && (strcmp(pcn->domainFreqList->head->name, ndfn->name )>0))){
            pcn -> domainFreqList -> head -> prev = ndfn;
            ndfn -> next = (pcn->domainFreqList->head);
            ndfn -> prev = NULL;
            pcn -> domainFreqList -> head = ndfn;
        }

        else if(pcn->domainFreqList->head->freq > ndfn->freq || (pcn->domainFreqList->head->freq == ndfn->freq && (strcmp(pcn->domainFreqList->head->name, ndfn->name )<0))){
            pcn->domainFreqList->head->next = ndfn;
            ndfn -> prev = pcn->domainFreqList->head;
            ndfn -> next = NULL;
            pcn->domainFreqList->tail = ndfn;
        }

        else{;}
    }

    else{
        if((pcn->domainFreqList->head)->freq < ndfn->freq || (pcn->domainFreqList->head->freq == ndfn->freq && (strcmp(pcn->domainFreqList->head->name, ndfn->name )>0))){
            pcn -> domainFreqList -> head -> prev = ndfn;
            ndfn -> next = (pcn->domainFreqList->head);
            ndfn -> prev = NULL;
            pcn -> domainFreqList -> head = ndfn;
        }
        

        else{
            pdfn = pcn->domainFreqList->head;
            cdfn = pdfn->next;
            while(cdfn && (cdfn->freq > ndfn->freq || (cdfn->freq == ndfn->freq && (strcmp(cdfn->name, ndfn->name )<0)))){
                pdfn = cdfn;
                cdfn = pdfn->next;
            }    

            pdfn->next =ndfn;
            ndfn->prev = pdfn;
            ndfn->next = cdfn;
            if(cdfn){
                cdfn->prev = ndfn;                      
            }
            else{
                pcn->domainFreqList->tail = ndfn;
            }    
        }
    }
}

/*
    Create a cache by reading inputs from stdin
*/
Cache* createCache()
{
    Cache *myCache;
    int i, cacheLimit, mediaCount;
    char *main_str, *token;

    main_str = (char*)malloc(sizeof(char) * 1001);
    myCache = (Cache*)malloc(sizeof(Cache));
    scanf("%d %d\n",&cacheLimit, &mediaCount);
    
    myCache->cacheLimit = cacheLimit;
    myCache->mediaCount = mediaCount;
    myCache->currentSize = 0;

    for(i=0;i<mediaCount;i++){

        int k = 0;
        int listindex = 0;
        CacheNode *ccn,*pcn;
        DomainFreqNode *(dfn_list)[501], *cdfn, *pdfn ,*ndfn;

        ccn = (CacheNode*)malloc(sizeof(CacheNode));

        if (i==0){
            myCache -> head = ccn;
            ccn -> prev = NULL;
            ccn -> next = NULL;
            pcn = ccn;
        }
        else{
            pcn -> next = ccn;
            ccn -> prev = pcn;
            ccn -> next = NULL;
            pcn = ccn; 
        }
        if(i == mediaCount-1){
            myCache->tail = pcn;
        }

        fgets(main_str, 1000, stdin);

        token = strtok(main_str, " ");
        pcn -> media.name = (char*)malloc(strlen(token)+1);
        strcpy(pcn -> media.name,token);

        token = strtok(NULL, " ");
        pcn -> media.size = atoi(token);
    
        myCache->currentSize += pcn->media.size;

        pcn->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));

        while((token = strtok(NULL, " "))){

            DomainFreqNode *dfn = (DomainFreqNode*)malloc(sizeof(DomainFreqNode)); 

            char* token2 = strtok(NULL, " ");
            dfn -> name = (char*)malloc(strlen(token)+1);
            strcpy(dfn -> name,token);
            dfn -> freq = atoi(token2);

            dfn_list[k] = dfn;
            k++;
        }

        for(; listindex < k; listindex++){

            ndfn = dfn_list[listindex];
            refreshlist(ndfn,pcn);
            
        }   
    }
    free(main_str);
    
    return myCache;
    }





void printCache(Cache* cache)
{
    DomainFreqNode *currfreq;
    CacheNode *curr;
    if(cache->head == NULL){
        printf("The Cache is Empty\n");
    }
    printf("-------------- Cache Information --------------\n");
    printf("Cache Limit: %d KB\nCurrent Size: %d KB\nMedia Count: %d\nCache Media Info:\n",cache->cacheLimit,cache->currentSize,cache->mediaCount);

    curr = cache->head;
    while(curr){
        printf("    Name: %s    Size: %d KB\n",curr->media.name,curr->media.size);
        printf("    ------- Media Domain Frequencies -------\n");   
        currfreq = curr->domainFreqList->head;
        while(currfreq){
            printf("        Name: %s    Freq: %d\n",currfreq->name,currfreq->freq);
            currfreq=currfreq->next; 
        }
        printf("    ----------------------------------------\n");
        curr = curr->next;
    }
    printf("----------------------------------------------");
}



CacheNode* addMediaRequest(Cache* cache, Media media, char* domain)
{
    CacheNode *curr, *prev, *next, *newcachenode, *tempnode;
    DomainFreqNode *cfn, *pfn, *nfn, *newfreqnode;
    int flag = 0;

    curr = cache->head;

    while (curr && strcmp(curr->media.name, media.name) != 0) {
        curr = curr->next;
    }
    if (curr) {
        prev = curr->prev;
        next = curr->next;
        if (prev && next) {
            prev->next = next;
            next->prev = prev;
        } 
        else if (prev && (next == NULL)) {
            prev->next = NULL;
            cache->tail = prev;
        }
        else{;}
        if (cache->head != curr) {
            cache->head->prev = curr;
            curr->next = cache->head;
            curr->prev = NULL;
            cache->head = curr;
        }

        cfn = curr->domainFreqList->head;

        while (cfn) {
            if (strcmp(cfn->name, domain) != 0) {
                cfn = cfn->next;
            }
            else {
                flag = 1;
                cfn->freq++;
                pfn = cfn->prev;
                nfn = cfn->next;
                if (pfn && nfn) {
                    pfn->next = nfn;
                    nfn->prev = pfn;
                    cfn->next = NULL;
                    cfn->prev = NULL;
                    refreshlist(cfn, curr);
                    return curr;
                } 
                else if (pfn == NULL) {
                    return curr;
                }
                else {
                    pfn->next = NULL;
                    cfn->next = NULL;
                    cfn->prev = NULL;
                    curr->domainFreqList->tail = pfn;
                    refreshlist(cfn, curr);
                    return curr;
                }
            }
        }

        if (flag != 1) {
            newfreqnode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));
            newfreqnode->name = (char*)malloc(strlen(domain)+1);
            strcpy(newfreqnode->name, domain);
            newfreqnode->freq = 1;
            refreshlist(newfreqnode, curr);
            return curr;
        }      
    } 

    else {
        newcachenode = (CacheNode*)malloc(sizeof(CacheNode));

        newfreqnode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));
        newfreqnode->prev = NULL;
        newfreqnode->next = NULL;
        newfreqnode->freq = 1;
        newfreqnode->name = (char*)malloc(strlen(domain)+1);
        strcpy(newfreqnode->name, domain);

        newcachenode->media.name = (char*)malloc(strlen(media.name) + 1);
        strcpy(newcachenode->media.name, media.name);
        newcachenode->media.size = media.size;
        newcachenode->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));
        newcachenode->domainFreqList->head = newfreqnode;
        newcachenode->domainFreqList->tail = newfreqnode;
        newcachenode->prev = NULL;

        while (cache->currentSize + media.size > cache->cacheLimit) {
            cache->currentSize -= cache->tail->media.size;
            cache->mediaCount--;
            tempnode = cache->tail;
            if(cache->head == cache->tail){
                cache->head = NULL;
                cache->tail = NULL;
                free(tempnode);
            }
            else{
                cache->tail = cache->tail->prev;
                cache->tail->next = NULL;
                free(tempnode);}
        }

        if (cache->tail) {
            newcachenode->next = cache->head;
            cache->head->prev = newcachenode;
            cache->head = newcachenode;
        } 
        else {
            cache->head = newcachenode;
            cache->tail = newcachenode;
            newcachenode->next = NULL;
        }

        cache->currentSize += media.size;
        cache->mediaCount++;

        return newcachenode;
    }
}


/*
    Finds the corresponding media's node in the cache
    Return NULL if it is not found
*/
CacheNode* findMedia(Cache* cache, char* name)
{
    CacheNode *curr;
    curr = cache->head;

    while(curr){
        if(strcmp(curr->media.name,name)!=0){
            curr = curr->next;
        }
        else{
            return curr;
        }
    }
    return NULL;
}

/*
    Finds a media from its name and deletes it's node from the cache
    Do not do anything if media is not found
*/
void deleteMedia(Cache* cache, char* name)
{
    CacheNode *prev,*curr,*next;
    prev = NULL;
    next = NULL;
    curr = cache->head;
    while(curr){
        if(strcmp(curr->media.name,name)!=0){
            prev = curr;
            curr = curr->next;
        }

        else{
            cache->mediaCount--;
            cache->currentSize -= curr->media.size;
            next = curr->next;
            
            if (next && (prev == NULL)){
                cache->head = next;
                next->prev = prev;}    
            else if (prev && (next == NULL)){
                prev->next = NULL;
                cache->tail = prev;}
            else{
                prev->next = next;
                next->prev = prev;}
            free(curr);
            break;
        }
    }
}
