# OS_7_2019
# INTERACTION OF PROCESSES THROUGH MEMORY SHARING
## Mục đích bài lab:  
Làm quen với công cụ trao đổi dữ liệu giữa các tiến trình (process)  - shared memory (bộ nhớ chia sẻ) và các lệnh hệ thống, đảm bảo việc tạo (create) shared memory, dẫn nó đến vùng nhớ cục bộ (map it to local memory), truyền dữ liệu (transfer data), đọc dữ liệu (read data), đóng (close) và xóa (delete) shred memory.  
## Mô tả lý thuyết  
Trong chuẩn POSIX khu vực bộ nhớ chia sẻ (a shared memory area) được tạo ra bởi lệnh sau:  
````int shm_open(const char *name, int oflag, mode_t mode),````  
trong đó:  
`name` – tên của khu vực bộ nhớ chia sẻ (name of shared memory area);  
`oflag` – flags, được định nghĩa kiểu của shared memory area;  
`mode` – quyền truy cập vào shared memory area.    

Thiết lập kích cỡ (size of shared memory area):  
````int ftruncate(int fd, off_t length),````
trong đó:  
`fd` - mô tả bộ nhớ chia sẻ thu được do kết quả của việc gọi hàm `shm_open ()` (shared memory descriptor obtained as a result of calling the `shm_open ()` function);  
`length` – kích cỡ yêu cầu của shared memory (the required size of shared memory).  

Việc nối các shared memory với vùng nhớ cục bộ (The mapping of shared memory to a local address):  
````
void *mmap(void *addr,size_t length,int prot,int flags,int fd,off_t offset),
````
trong đó:  
`addr` - ánh xạ địa chỉ bắt đầu (mapping start address);  
`length` - kích cỡ của ánh xạ (size of the display);  
`prot` - một tham số xác định quyền đọc / ghi cho ánh xạ (a parameter that determines read / write permission for a display);  
`flags` - một tham số xác định các quy tắc cho khả năng hiển thị của các processes (a parameter that defines the rules for the visibility of the display of processes);  
`fd` - is the shared memory descriptor;  
`offset` - offset on the part of the shared memory relative to the starting address.  

Xóa map của shared memory vào địa chỉ cục bộ:  
````int munmap(void *addr, size_t length),````  
trong đó:   
`addr` – local mapping address;  
`length` -  size of mapping.   

Đóng shared memory area:  
````int close(int fd),````
trong đó:  
`fd` - descriptor.  

Xóa shared memory area:  
````int shm_unlink(const char *name),````
trong đó:  
`name` – name of the shared memory location.  

Trong chuẩn SVID thì shared memory area:  
````int shmget (key_t key, int size, int shmflg);````
trong đó:
`key_t key` - the key obtained by the `ftok ()` function;  
`int size` - required memory size;  
`int shmflg` - flags that set memory access rights, for example, 0644 | IPC_CREAT.  

Sau khi tạo một phần bộ nhớ dùng chung, nó phải được kết nối với không gian địa chỉ của quy trình. Điều này được thực hiện bằng cách gọi:  
````void * shmat (int shmid, const void * shmaddr, int shmflg);````
trong đó:  
`int shmid` - segment identifier;  
`const void * shmaddr` - memory address;  
`int shmflg` - flags that set memory access rights.  

Sau khi sử dụng bộ nhớ, nó phải được ngắt kết nối khỏi không gian địa chỉ của quá trình bằng cách gọi:  
````int shmdt (const void * shmaddr);````
trong đó:  
`const void * shmaddr` - memory address;  
Và sau đó xóa bằng lệnh:  
````int shmctl (int shmid, int cmd, struct shmid_ds * buf);````
trong đó:  
`int shmid` - segment identifier;  
`int cmd` - command code, IPC_RMID is used for deletion;  
`struct shmid_ds * buf` is a structure for storing information about a segment of shared memory; it is not used in case of deletion.  

## Yêu cầu:  
Viết một bộ gồm hai chương trình, một trong số đó gửi dữ liệu vào bộ nhớ dùng chung và chương trình thứ hai đọc dữ liệu này. Do cơ chế bộ nhớ dùng chung không chứa các công cụ đồng bộ hóa đọc / đọc, nên cần sử dụng cơ chế semaphore có tên để đồng bộ hóa.  
* Program 1:  
````declare a flag to end the thread;
declare the semaphore id of the record;
declare the semaphore id of the reading;
declare shared memory ID;
announce local address;
flow function ()
{
declare a variable;
yet (the thread termination flag is not set)
{
assign a random value to a variable;
display the variable value on the screen;
copy the value of the variable to the local address;
release semaphore record;
wait for the semaphore reading;
delay for a while;
}
}
main program ()
{
declare a thread id;
create (or open if exists) shared memory;
cut shared memory to the required size;
map shared memory to local address;
create (or open, if exists) a record semaphore;
create (or open, if exists) a reading semaphore;
create a stream from the stream function;
wait for keypress;
set the end of flow flag;
wait for the thread to complete;
close reading semaphore;
remove reading semaphore;
close semaphore record;
delete semaphore record;
close the mapping of shared memory to a local address;
remove shared memory;
}
````
* Program 2:  
````declare a flag to end the thread;
declare the semaphore id of the record;
declare the semaphore id of the reading;
declare shared memory ID;
announce local address;
flow function ()
{
declare a variable;
yet (the thread termination flag is not set)
{
wait for the semaphore record;
copy data from a local address to a variable;
display the variable value on the screen;
release the reading semaphore;
}
}
main program ()
{
declare a thread id;
create (or open if exists) shared memory;
change the size of shared memory to the required;
map shared memory to local address;
create (or open, if exists) a record semaphore;
create (or open, if exists) a reading semaphore;
create a stream from the stream function;
wait for keypress;
set the end of flow flag;
wait for the thread to complete;
close reading semaphore;
remove reading semaphore;
close semaphore record;
delete semaphore record;
close the mapping of shared memory to a local address;
remove shared memory;
}
````




