# semaphore_application

các file trong repo này độc lập, không liên quan với nhau

1. semaphore_implementation.cpp
- implement semaphore sử dụng mutex lock
- semaphore được cải tiến từ mutex, cho phép một hoặc nhiều luồng truy cập vào tài nguyên chung (thay vì chỉ một luồng như mutex)
- semaphore sử dụng mutex để lock một đoạn code, và một biến condition được sử dụng cho các hàm của thread như pthread_cond_wait và pthread_cond_signal (dùng để tạm thả mutex, cũng như đánh thức thread đang bị block), biến value là giá trị ban đầu của semaphore (thường là số lượng các luồng được truy cập vào tài nguyên chung trong cùng một thời điểm)

2. producer_consumer.cpp
- ứng dụng của semaphore
- chặn luồng producer nếu buffer đã full
- chặn luồng consumer nếu buffer đang empty

3. login_queue.cpp
- ứng dụng của semaphore
- mô tả quá trình player đăng nhập vào ứng dụng, nếu số lượng player vượt quá số lượng server có thể xử lý, player đó bị chặn (block) cho đến khi một tiến trình khác thoát khỏi ứng dụng

