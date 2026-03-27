#include <stdio.h>
#include <string.h>

#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "phonebook.h"

//чтение строки
static void read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';       //убираем \n
    } else {
        buffer[0] = '\0';
    }
}

//чтение положительного числа
static unsigned int read_uint(const char *prompt) {
    char line[64];
    unsigned int value;

    while (1) {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL) {     //читаем строку
            return 0;
        }

        if (sscanf(line, "%u", &value) == 1) {      //ищем число
            return value;
        }

        printf("Ошибка ввода. Введите число.\n");
    }
}

static void print_contact(const Contact *c) {
    printf("\nID: %u\n", c->id);
    printf("Фамилия: %s\n", c->surname);
    printf("Имя: %s\n", c->name);
    printf("Отчество: %s\n", c->middle_name);
    printf("Место работы: %s\n", c->workplace);
    printf("Должность: %s\n", c->position);
    printf("Телефон: %s\n", c->other.phone);
    printf("Email: %s\n", c->other.email);
    printf("Соцсеть: %s\n", c->other.social);
    printf("Мессенджер: %s\n", c->other.messenger);
}

static void print_all_contacts(const ContactList *list) {
    size_t i;

    if (list->size == 0) {
        printf("\nСписок контактов пуст.\n");
        return;
    }

    for (i = 0; i < list->size; i++) {
        print_contact(&list->data[i]);
    }
}

static void add_contact_ui(ContactList *list) {
    char surname[STR_LEN];
    char name[STR_LEN];
    char middle_name[STR_LEN];
    char workplace[STR_LEN];
    char position[STR_LEN];
    char phone[STR_LEN];
    char email[STR_LEN];
    char social[STR_LEN];
    char messenger[STR_LEN];

    read_line("Фамилия: ", surname, sizeof(surname));
    read_line("Имя: ", name, sizeof(name));

    if (surname[0] == '\0' || name[0] == '\0') {
        printf("Фамилия и имя обязательны.\n");
        return;
    }

    read_line("Отчество: ", middle_name, sizeof(middle_name));
    read_line("Место работы: ", workplace, sizeof(workplace));
    read_line("Должность: ", position, sizeof(position));
    read_line("Телефон: ", phone, sizeof(phone));
    read_line("Email: ", email, sizeof(email));
    read_line("Ссылка на соцсеть: ", social, sizeof(social));
    read_line("Мессенджер: ", messenger, sizeof(messenger));

    if (add_contact(list, surname, name, middle_name, workplace, position, phone, email, social, messenger)) {
        printf("Контакт добавлен.\n");
    } else {
        printf("Не удалось добавить контакт.\n");
    }
}

static void delete_contact_ui(ContactList *list) {
    unsigned int id = read_uint("Введите ID для удаления: ");

    if (delete_contact(list, id)) {
        printf("Контакт удалён.\n");
    } else {
        printf("Контакт с таким ID не найден.\n");
    }
}

static void find_by_surname_ui(const ContactList *list) {
    char surname[STR_LEN];
    size_t indexes[100];
    int found;
    int i;

    read_line("Введите фамилию: ", surname, sizeof(surname));
    found = find_by_surname(list, surname, indexes, 100);

    if (found == 0) {
        printf("Контакты не найдены.\n");
        return;
    }

    for (i = 0; i < found && i < 100; i++) {
        print_contact(&list->data[indexes[i]]);
    }
}

static void edit_contact_ui(ContactList *list) {
    unsigned int id;
    Contact *c;
    unsigned int field;
    char value[STR_LEN];

    id = read_uint("Введите ID контакта: ");
    c = find_contact_by_id(list, id);

    if (c == NULL) {
        printf("Контакт не найден.\n");
        return;
    }

    print_contact(c);

    printf("\nЧто изменить?\n");
    printf("1. Фамилия\n");
    printf("2. Имя\n");
    printf("3. Отчество\n");
    printf("4. Место работы\n");
    printf("5. Должность\n");
    printf("6. Телефон\n");
    printf("7. Email\n");
    printf("8. Соцсеть\n");
    printf("9. Мессенджер\n");

    field = read_uint("Выберите поле: ");
    read_line("Введите новое значение: ", value, sizeof(value));

    if (update_contact_fields(c, 1, field, value)) {
        printf("Контакт изменён.\n");
    } else {
        printf("Ошибка изменения.\n");
    }
}



int main(void) {
    ContactList list;
    unsigned int choice;

    setlocale(LC_ALL, "ru_RU.UTF-8");
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    init_contact_list(&list);

    while (1) {
        printf("\n------ ТЕЛЕФОННАЯ КНИГА ------\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать все контакты\n");
        printf("3. Найти по фамилии\n");
        printf("4. Изменить контакт\n");
        printf("5. Удалить контакт\n");
        printf("6. Сортировать по алфавиту\n");
        printf("0. Выход\n");

        choice = read_uint("Выберите пункт: ");

        switch (choice) {
            case 1:
                add_contact_ui(&list);
                break;

            case 2:
                print_all_contacts(&list);
                break;

            case 3:
                find_by_surname_ui(&list);
                break;

            case 4:
                edit_contact_ui(&list);
                break;

            case 5:
                delete_contact_ui(&list);
                break;
                
            case 6:
                sort_contacts(&list, compare_by_surname_name);
                printf("Список отсортирован.\n");
                break;
            
            case 0:
                free_contact_list(&list);
                printf("Выход.\n");
                return 0;
            
            default:
                printf("Неверный пункт меню.\n");
        }
    }
}