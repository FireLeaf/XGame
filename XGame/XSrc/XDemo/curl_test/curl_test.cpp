// curl_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
// extern "C"
// {
	#include "curl/curl.h"
//}
#pragma comment(lib, "libcurl_d.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	 CURL *curl;
 
  CURLM *multi_handle;
  int still_running;
 
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
 
  /* Fill in the file upload field. This makes libcurl load data from
     the given file name when curl_easy_perform() is called. */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "sendfile",
               CURLFORM_FILE, "postit2.c",
               CURLFORM_END);
 
  /* Fill in the filename field */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename",
               CURLFORM_COPYCONTENTS, "postit2.c",
               CURLFORM_END);
 
  /* Fill in the submit field too, even if this is rarely needed */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "submit",
               CURLFORM_COPYCONTENTS, "send",
               CURLFORM_END);
 
  curl = curl_easy_init();
  multi_handle = curl_multi_init();
 
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */ 
  headerlist = curl_slist_append(headerlist, buf);
  if(curl && multi_handle) {
 
    /* what URL that receives this POST */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.gameres.com/data/attachment/forum/201411/17/184119uzkzk6emmwounnuw.jpg");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
 
    curl_multi_add_handle(multi_handle, curl);
 
    curl_multi_perform(multi_handle, &still_running);
 
    do {
      struct timeval timeout;
      int rc; /* select() return code */ 
      CURLMcode mc; /* curl_multi_fdset() return code */ 
 
      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      int maxfd = -1;
 
      long curl_timeo = -1;
 
      FD_ZERO(&fdread);
      FD_ZERO(&fdwrite);
      FD_ZERO(&fdexcep);
 
      /* set a suitable timeout to play around with */ 
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
 
      curl_multi_timeout(multi_handle, &curl_timeo);
      if(curl_timeo >= 0) {
        timeout.tv_sec = curl_timeo / 1000;
        if(timeout.tv_sec > 1)
          timeout.tv_sec = 1;
        else
          timeout.tv_usec = (curl_timeo % 1000) * 1000;
      }
 
      /* get file descriptors from the transfers */ 
      mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
 
      if(mc != CURLM_OK)
      {
        fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
        break;
      }
 
      /* On success the value of maxfd is guaranteed to be >= -1. We call
         select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
         no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
         to sleep 100ms, which is the minimum suggested value in the
         curl_multi_fdset() doc. */ 
 
      if(maxfd == -1) {
#ifdef _WIN32
        Sleep(100);
        rc = 0;
#else
        /* Portable sleep for platforms other than Windows. */ 
        struct timeval wait = { 0, 100 * 1000 }; /* 100ms */ 
        rc = select(0, NULL, NULL, NULL, &wait);
#endif
      }
      else {
        /* Note that on some platforms 'timeout' may be modified by select().
           If you need access to the original value save a copy beforehand. */ 
        rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
      }
 
      switch(rc) {
      case -1:
        /* select error */ 
        break;
      case 0:
      default:
        /* timeout or readable/writable sockets */ 
        printf("perform!\n");
        curl_multi_perform(multi_handle, &still_running);
        printf("running: %d!\n", still_running);
        break;
      }
    } while(still_running);
 
    curl_multi_cleanup(multi_handle);
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* then cleanup the formpost chain */ 
    curl_formfree(formpost);
 
    /* free slist */ 
    curl_slist_free_all (headerlist);
  }
	return 0;
}

