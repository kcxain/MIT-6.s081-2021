#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
find(char *path, char *file)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    switch (st.type) {
    case T_DIR:
      if(strcmp(de.name, ".") != 0 && strcmp(de.name, ".."))
        find(buf, file);
    case T_FILE:
      if (strcmp(de.name, file) == 0) {
				printf("%s\n", buf);
			}
    }
  }
  close(fd);
  return;
}

int
main(int argc, char *argv[])
{
  /* 未指定目录, 则在当前目录下查找 */
  if(argc == 2){
    find(".", argv[1]);
  }
  else if(argc == 3){
    find(argv[1], argv[2]);
  }
  else{
    fprintf(2, "find: find <dir> <file>\n");
    exit(1);
  }
  exit(0);
}