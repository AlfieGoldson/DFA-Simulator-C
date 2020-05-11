#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_state_accepted(char *state, char *accepting_states)
{
    for (int i = 0; i < sizeof(accepting_states) / sizeof(accepting_states[0]); i++)
        if (accepting_states[i] == *state)
            return 1;
    return 0;
}

void get_states(char delim, char *buf, char character, int states[])
{
    size_t i = 0;
    int state = 0;
    int states_count = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == delim)
            state++;
        else if (buf[i] == character)
            states[states_count++] = state;
        i++;
    }
}

int test_word(char alphabet[], int state, char accepting_states[], int states_count, char transitions[states_count][states_count], char *word)
{
    if (strlen(word) == 0)
    {
        char state_str[256];
        sprintf(state_str, "%d", state);
        return is_state_accepted(state_str, accepting_states);
    }

    // Check w[0] in alphabet

    int new_states[5] = {[0 ... 4] = -1};

    get_states(';', transitions[state], word[0], new_states);

    memmove(&word[0], &word[1], strlen(word));

    for (size_t i = 0; i < 5; i++)
    {
        if (new_states[i] == -1)
            break;

        if (test_word(alphabet, new_states[i], accepting_states, states_count, transitions, word) == 1)
            return 1;
    }
    return 0;
}

int main()
{
    char file_name[256];
    char mot[256];

    printf("Nom du fichier décrivant l'automate:\n");
    scanf("%s", file_name);
    printf("Mot:\n");
    scanf("%s", mot);

    FILE *fp;
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    char line[60];
    char alphabet[256];
    int state, states_count;
    char accepting_states[256];

    // Alphabet
    fgets(line, 60, fp);
    strcpy(alphabet, line);

    // État Initial
    fgets(line, 60, fp);
    state = atoi(line);

    // États Acceptants
    fgets(line, 60, fp);
    strcpy(accepting_states, line);

    // Nombre d'États
    fgets(line, 60, fp);
    states_count = atoi(line);

    // Fonctions de Transition
    char transitions[states_count][states_count];
    for (int i = 0; i < states_count; i++)
    {
        fgets(line, 60, fp);
        strcpy(transitions[i], line);
    }

    fclose(fp);
    printf("%s => ", mot);
    printf("%s\n", test_word(alphabet, state, accepting_states, states_count, transitions, mot) == 0 ? "NON" : "OUI");

    return 0;
}
