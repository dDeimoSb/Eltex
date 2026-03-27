#include "phonebook.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void safe_copy(char *dest, const char *src) {
    if (src == NULL) {
        dest[0] = '\0';
        return;
    }

    strncpy(dest, src, STR_LEN - 1);
    dest[STR_LEN - 1] = '\0';
}

static int check_capacity(ContactList *list) {
    Contact *tmp;
    size_t new_capacity;

    if (list->size < list->capacity) {
        return 1;
    }

    new_capacity = (list->capacity == 0) ? 1 : list->capacity + 50;  //--------------------------------------------

    tmp = (Contact *)realloc(list->data, new_capacity * sizeof(Contact));
    if (tmp == NULL) {
        return 0;
    }

    list->data = tmp;
    list->capacity = new_capacity;
    return 1;
}

void init_contact_list(ContactList *list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
    list->next_id = 1;
}

void free_contact_list(ContactList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
    list->next_id = 1;
}

int add_contact(ContactList *list,
                const char *surname,
                const char *name,
                const char *middle_name,
                const char *workplace,
                const char *position,
                const char *phone,
                const char *email,
                const char *social,
                const char *messenger) {
    Contact *c;

    if (surname == NULL || name == NULL || surname[0] == '\0' || name[0] == '\0') {     //проверка заполнения необъходимых параметров
        return 0;
    }

    if (!check_capacity(list)) {       //проверка места
        return 0;
    }

    c = &list->data[list->size];        //адрес нового места

    c->id = list->next_id++;

    safe_copy(c->surname, surname);
    safe_copy(c->name, name);
    safe_copy(c->middle_name, middle_name);
    safe_copy(c->workplace, workplace);
    safe_copy(c->position, position);
    safe_copy(c->other.phone, phone);
    safe_copy(c->other.email, email);
    safe_copy(c->other.social, social);
    safe_copy(c->other.messenger, messenger);

    list->size++;

    return 1;
}

int delete_contact(ContactList *list, unsigned int id) {
    size_t i;
    
    for (i = 0; i < list->size; i++) {
        if (list->data[i].id == id) {
            size_t j;
            for (j = i; j + 1 < list->size; j++) {
                list->data[j] = list->data[j + 1];      //сдвиг элементов
            }

            list->size--;
            return 1;
        }
    }

    return 0;
}

Contact *find_contact_by_id(ContactList *list, unsigned int id) {
    size_t i;

    for (i = 0; i < list->size; i++) {
        if (list->data[i].id == id) {
            return &list->data[i];      //адрес контакта
        }
    }

    return NULL;
}

int find_by_surname(const ContactList *list,
                    const char *surname,
                    size_t *indexes,
                    size_t max_indexes) {
    size_t i;
    int count = 0;

    if (list == NULL || surname == NULL || indexes == NULL) {       //проверка
        return 0;
    }

    for (i = 0; i < list->size; i++) {
        if (strcmp(list->data[i].surname, surname) == 0) {
            if ((size_t)count < max_indexes) {      //проверка чтобы не выйти за границы массива индексов
                indexes[count] = i;
            }
            count++;
        }
    }

    return count;
}

int compare_by_surname_name(const Contact *a, const Contact *b) {
    int cmp;

    cmp = strcmp(a->surname, b->surname);       //проверка по фамилиям
    if (cmp != 0) {     //разные фамилии
        return cmp;     //результат сравнения
    }

    cmp = strcmp(a->name, b->name);
    if (cmp != 0) {
        return cmp;
    }
    //сравнение по id если фамилия имя одинаковые
    if (a->id < b->id) return -1;       //а раньше
    if (a->id > b->id) return 1;
    return 0;
}

void sort_contacts(ContactList *list, ContactCompare cmp) {
    size_t i, j;

    if (cmp == NULL) {      //проверка
        return;
    }

    for (i = 0; i < list->size; i++) {      //пузырек
        for (j = 0; j + 1 < list->size - i; j++) {
            if (cmp(&list->data[j], &list->data[j + 1]) > 0) {      //> 0, то первый после второго
                Contact temp = list->data[j];
                list->data[j] = list->data[j + 1];
                list->data[j + 1] = temp;
            }
        }
    }
}

int update_contact_fields(Contact *contact, unsigned int num, ...) {     //функция с переменным числом параметров
    va_list args;       //для ...
    unsigned int i;

    if (contact == NULL || num <= 0) {      //проверка
        return 0;
    }

    va_start(args, num);        //чтение переменных агрументов

    for (i = 0; i < num; i++) {     //по количеству изменяемых полей
        int field = va_arg(args, int);                      //номер поля
        const char *value = va_arg(args, const char *);     //значение

        if (value == NULL) {
            value = "";
        }

        switch (field) {
            case FIELD_SURNAME:                         //обязательно для заполнения
                if (value[0] != '\0') {                 //если не пустое
                    safe_copy(contact->surname, value);
                }
                break;
            case FIELD_NAME:                            //обязательно для заполнения
                if (value[0] != '\0') {
                    safe_copy(contact->name, value);
                }
                break;
            
            case FIELD_MIDDLE_NAME:
                safe_copy(contact->middle_name, value);
                break;
            case FIELD_WORKPLACE:
                safe_copy(contact->workplace, value);
                break;
            case FIELD_POSITION:
                safe_copy(contact->position, value);
                break;
            case FIELD_PHONE:
                safe_copy(contact->other.phone, value);
                break;
            case FIELD_EMAIL:
                safe_copy(contact->other.email, value);
                break;
            case FIELD_SOCIAL:
                safe_copy(contact->other.social, value);
                break;
            case FIELD_MESSENGER:
                safe_copy(contact->other.messenger, value);
                break;

            default:
                va_end(args);
                return 0;
        }
    }

    va_end(args);       //конец работы с переменным числом параметров
    return 1;
}