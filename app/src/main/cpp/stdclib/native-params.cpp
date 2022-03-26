//
// Created by WHF on 2022/3/14.
// 参数传递
//

#include <jni.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include "../utils/log.h"

class Person {
public:
    //构造函数
    Person(std::string name) {
        LOGD("call create %s", name.c_str());
        this->name = name;
        this->bufferSize = 10;
        this->buffer = (char *) malloc(10);
    }

    //拷贝构造函数，在参数传递的时候会调用
    Person(const Person &person) {
        LOGD("call copy create");
        //深拷贝
        this->name = person.name;
        this->bufferSize = person.bufferSize;
        this->buffer = (char *) malloc(bufferSize);
        memcpy(buffer, person.buffer, bufferSize);
    }

    //浅拷贝
    Person(Person &&person) {
        LOGD("call copy create2");
        this->name = person.name;
        this->buffer = person.buffer;
        this->bufferSize = person.bufferSize;
        person.buffer = nullptr;
    }

    Person operator=(Person person) {
        LOGD("call operator = ");
        Person person1(person.name);
        return person1;
    };

    ~Person() {
        LOGD("release person %s", name.c_str());
    }

    char *buffer;
    int bufferSize;
    std::string name;
};

void testPointParams(Person *person) {
    LOGD("===testPointParams=== person name=%s addr=%p", person->name.c_str(), person);
    person->name = "指针传递修改";
}

void testValueParams(Person person) {
    LOGD("===testValueParams=== person name=%s addr=%p", person.name.c_str(), &person);
    person.name = "值传递修改";
}

void testRefParams(Person &person) {
    LOGD("===testRefParams=== person name=%s addr=%p", person.name.c_str(), &person);
    person.name = "引用传递修改";
}

void testSpValueParams(std::shared_ptr<Person> person) {
    LOGD("===testSpValueParams=== person name=%s addr=%p spCount=%ld", person->name.c_str(),
         &person, person.use_count());
    person->name = "智能指针值传递修改";
}

void testSpPointParams(std::shared_ptr<Person> *person) {
    LOGD("===testSpPointParams=== person name=%s addr=%p spCount=%ld", (*person)->name.c_str(),
         person, (*person).use_count());
    (*person)->name = "智能指针指针传递修改";
}

void testSpRefParams(std::shared_ptr<Person> &person) {
    LOGD("===testSpPointParams=== person name=%s addr=%p spCount=%ld", person->name.c_str(),
         &person, person.use_count());
    person->name = "智能指针引用传递修改";
}

//传递的实参会被回收掉
void *testThread(void *person) {
    Person *person1 = (Person *) person;
    sleep(5);
    LOGD("===testPointParams=== person name=%s addr=%p", person1->name.c_str(), person);
    person1->name = "多线程指针传递修改";
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_nativeParams(
        JNIEnv *env,
        jobject /* this */) {
    Person person("原始值");
    LOGD("person name=%s addr=%p", person.name.c_str(), &person);

    //右值引用
    Person &&rightPerson = std::move(person);
    //浅拷贝（移动语意）
    Person copyPerson(rightPerson);
    //深拷贝
    Person copyPerson2(person);

    //测试值传递
    testValueParams(person);
    LOGD("person name=%s addr=%p", person.name.c_str(), &person);

    //测试指针传递
    testPointParams(&person);
    LOGD("person name=%s addr=%p", person.name.c_str(), &person);

    testRefParams(person);
    LOGD("person name=%s addr=%p", person.name.c_str(), &person);

    std::shared_ptr<Person> personSp = std::make_shared<Person>("智能指针原始值");
    LOGD("person name=%s addr=%p spCount=%ld", personSp->name.c_str(), &personSp,
         personSp.use_count());

    //引用计数+1，所以智能指针必须值传递，因为智能指针重写了拷贝构造函数
    testSpValueParams(personSp);
    LOGD("person name=%s addr=%p spCount=%ld", personSp->name.c_str(), &personSp,
         personSp.use_count());

    //引用计数并没有+1，所以智能指针不要指针传递
    testSpPointParams(&personSp);
    LOGD("person name=%s addr=%p spCount=%ld", personSp->name.c_str(), &personSp,
         personSp.use_count());

    //引用计数并没有+1，所以智能指针不要引用传递
    testSpRefParams(personSp);
    LOGD("person name=%s addr=%p spCount=%ld", personSp->name.c_str(), &personSp,
         personSp.use_count());

    //多线程中person2会被回收，所以指针传递在多线程是不安全的
    Person person2("线程测试");
    pthread_t *thread;
    pthread_create(thread, NULL, testThread, &person);

    //多线程中person2会被回收，所以指针传递在多线程是不安全的
    std::shared_ptr<Person> person3Sp = std::make_shared<Person>("多线程智能指针原始值");
    pthread_t thread2;
    pthread_create(&thread2, NULL, testThread, &person3Sp);
}
