#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <errno.h>

#ifdef NDEBUG
#define LOG_DEBUG(M, ...)
#else
#define LOG_DEBUG(M, ...) \
  fprintf(stderr, "[DEBUG] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define LOG_ERR(M, ...)                                                     \
  fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
          errno == 0 ? "None" : strerror(errno), ##__VA_ARGS__)

#define LOG_INFO(M, ...) \
  fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define CHECK(COND)                        \
  do {                                     \
    if (!(COND)) {                         \
      LOG_ERR("Check failure: %s", #COND); \
      exit(-1);                            \
    }                                      \
  } while (0);

#define UNIQUE_ID_SIZE 20

/* Cleanup method for running tests with the greatest library.
 * Runs the test, then clears the Redis database. */
#define RUN_REDIS_TEST(context, test) \
  RUN_TEST(test);                     \
  freeReplyObject(redisCommand(context, "FLUSHALL"));

typedef struct { unsigned char id[UNIQUE_ID_SIZE]; } unique_id;

/* Generate a globally unique ID. */
unique_id globally_unique_id(void);

/* Convert a 20 byte sha1 hash to a hexdecimal string. This function assumes
 * that buffer points to an already allocated char array of size 2 *
 * UNIQUE_ID_SIZE + 1 */
char *sha1_to_hex(const unsigned char *sha1, char *buffer);

/* Convert a hexdecimal string of length 40 to a 20 byte sha1 hash. This
 * function assumes that sha1 points to an already allocated char array of size
 * UNIQUE_ID_SIZE. */
int hex_to_sha1(const char *hex, unsigned char *sha1);

typedef unique_id object_id;

#endif
