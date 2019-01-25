#include "string_buffer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main
(void)
{
  string_buffer strbuf;
  init_strbuf(&strbuf);

  assert(("Buffer size initialized to 0", strbuf_size(&strbuf) == 0));

  strbuf_append_str(&strbuf, "Hello");
  assert(("Buffer == \"Hello\"", strcmp("Hello", strbuf_data(&strbuf)) == 0));

  strbuf_append_str(&strbuf, ", world");
  assert(("Buffer == \"Hello, world\"", strcmp("Hello, world", strbuf_data(&strbuf)) == 0));

  strbuf_append_ch(&strbuf, '!');
  assert(("Buffer == \"Hello, world!\"", strcmp("Hello, world!", strbuf_data(&strbuf)) == 0));

  char * str = cpy_free_strbuf(&strbuf);
  assert(("str == \"Hello, world!\"", strcmp("Hello, world!", str) == 0));

  free(str);
  return 0;
}