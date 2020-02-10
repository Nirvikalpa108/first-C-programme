#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/curl/curl.h"

char* extract_string(char* json_data, jsmntok_t token) {
  int string_length = token.end - token.start;
  // work out how long the string is going to be
  char* string = malloc(string_length + 1);
  // copy some data to `string`
  char* starting_slot = json_data + token.start; 
  memcpy(string, starting_slot, string_length);
  string[string_length] = 0;
  return string;
}

char* get_value(char* json_data, jsmntok_t* token, char* key) {
  return 0;
  // iterate over all the keys in the object, until we get all the ones we want

  // define target key (value is a string)
  // for loop
  // check each key
  // if it satisfies target key requirement
  // run the extract string function and print the word
}

              // for (int i = 4; i < number_total_tokens; i+=2) 
      //   {
      //   char slots_required[200]; 
      //   char* starting_slot = api_response + tokens[i].start;
      //   int length = tokens[i].end - tokens[i].start;
      //   memcpy(slots_required, starting_slot, length);
      //   slots_required[length] = 0;
      //   printf("%s\n", slots_required);
      // }

void parser(char* api_response, size_t size)
{
    jsmn_parser parser;
    jsmntok_t tokens[128]; /* We expect no more than 128 JSON tokens */

    jsmn_init(&parser);
    int number_total_tokens = jsmn_parse(&parser, api_response, size, tokens, 128);
    printf("%d number of tokens\n", number_total_tokens);

    printf("%s\n", extract_string(api_response, tokens[4]));

    for (int i = 0; i < number_total_tokens; i++) 
    {
        printf("start %d, end %d, size %d, type %d \n", 
            tokens[i].start, 
            tokens[i].end, 
            tokens[i].size, 
            tokens[i].type);
    }

      //  char api_response_word[20]; // allocating chars to store api response word
      //  char* starting_slot = api_response + tokens[1].start; 
      //  int length = tokens[1].end - tokens[1].start;
      //  memcpy(api_response_word, starting_slot, length);
      //  api_response_word[length] = 0;
      //  printf("%s successfully parsed\n", api_response_word);

      // for (int i = 0; i < number_total_tokens; i++) 
      // {
      //   char slots_required[200]; 
      //   char* starting_slot = api_response + tokens[i].start;
      //   int length = tokens[i].end - tokens[i].start;
      //   memcpy(slots_required, starting_slot, length);
      //   slots_required[length] = 0;
      //   printf("%s\n", slots_required);
      // }

      // for (int i = 4; i < number_total_tokens; i+=2) 
      //   {
      //   char slots_required[200]; 
      //   char* starting_slot = api_response + tokens[i].start;
      //   int length = tokens[i].end - tokens[i].start;
      //   memcpy(slots_required, starting_slot, length);
      //   slots_required[length] = 0;
      //   printf("%s\n", slots_required);
      // }
}

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
 
int main(void)
{
  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */ 
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, "https://content.guardianapis.com/?api-key=gnm-hackday-20&page-size=1");
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
  /* get it! */ 
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    // parser(chunk.memory, chunk.size);
    parser("{\"status\": \"ok\", \"userTier\": \"internal\", \"total\": 1}", strlen("{\"status\": \"ok\", \"userTier\": \"internal\", \"total\": 1}")); 
    printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  free(chunk.memory);
 
  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();
 
  return 0;
}