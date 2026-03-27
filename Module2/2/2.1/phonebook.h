#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <stddef.h>

#define STR_LEN 100 //длина строки

typedef struct {
    char phone[STR_LEN];        //номер телефона
    char email[STR_LEN];        //адрес эл. почты
    char social[STR_LEN];       //страница в соц. сетях
    char messenger[STR_LEN];    //профиль в мессенджерах
} Communication;

typedef struct {
    unsigned int id;
    char surname[STR_LEN];      //фамилия
    char name[STR_LEN];         //имя
    char middle_name[STR_LEN];  //отчество
    char workplace[STR_LEN];    //место работы
    char position[STR_LEN];     //должность
    Communication other;
} Contact;

typedef struct {
    Contact *data;
    size_t size;
    size_t capacity;
    unsigned int next_id;
} ContactList;

typedef int (*ContactCompare)(const Contact *, const Contact *);    //указатель на функцию

enum ContactField {
    FIELD_SURNAME       = 1,
    FIELD_NAME          = 2,
    FIELD_MIDDLE_NAME   = 3,
    FIELD_WORKPLACE     = 4,
    FIELD_POSITION      = 5,
    FIELD_PHONE         = 6,
    FIELD_EMAIL         = 7,
    FIELD_SOCIAL        = 8,
    FIELD_MESSENGER     = 9
};

//инициализация
void init_contact_list(ContactList *list);
//освобождение памяти
void free_contact_list(ContactList *list);

//добавление контакта
int add_contact(ContactList *list,
                const char *surname,
                const char *name,
                const char *middle_name,
                const char *workplace,
                const char *position,
                const char *phone,
                const char *email,
                const char *social,
                const char *messenger);

//удаление контакта
int delete_contact(ContactList *list, unsigned int id);

//поиск по id
Contact *find_contact_by_id(ContactList *list, unsigned int id);

//поиск по фамилии
int find_by_surname(const ContactList *list,
                    const char *surname,
                    size_t *indexes,
                    size_t max_indexes);

// сравнение для сортировки
int compare_by_surname_name(const Contact *a, const Contact *b);

//сортировка
void sort_contacts(ContactList *list, ContactCompare cmp);          //через указатель на функцию сравнения

//редактирование контакта
int update_contact_fields(Contact *contact, unsigned int num, ...);      //функция с переменным числом параметров

#endif