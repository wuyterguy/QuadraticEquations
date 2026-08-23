Input_type AskInputType() {
    printf("Quadratic equations solver by " Color("Zotov Anton", YEL) "\n");
    printf("Would you like to use file input from file? (write "
           Color("Y", MAG) " or " Color("N", MAG) ")\n");
    printf("If you write N it will be console input\n");

    Answer ask_result = AskYesOrNo();
    if (ask_result == YES)
        return FILE_INPUT;
    else if (ask_result == NO)
        return CONSOLE_INPUT;
    else
        return ERROR;
}


Freq_err FileRequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c) {
    assert(p_coef_a != NULL);
    assert(p_coef_b != NULL);
    assert(p_coef_c != NULL);
    assert(p_coef_a != p_coef_b && p_coef_a != p_coef_c && p_coef_b != p_coef_c);

    Freq_err error_identification = FREQ_NORMAL;

    while(1) {                                          // request the correct file unless input is possible to read 3 coefficients or program is closed
        printf("Enter a filename like: " Color("coefficients.txt", YEL) "\n");

        char* filename = GetFilename();
        FILE* coefficients_file = fopen(filename, "r");

        error_identification = FREQ_NORMAL;

        if (coefficients_file == NULL)
            error_identification = FREQ_FOPEN_ERROR;
        else { /* (coefficients_file != NULL) */
            if (fscanf(coefficients_file, "%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3)
                error_identification = FREQ_INPUT_ERROR;

            if (ferror(coefficients_file)) {
                printf("File " Color("%s", YEL) " - error during the reading\n", filename);
                fclose(coefficients_file);
                free(filename);
                return FREQ_READ_ERROR;
            }
        }

        if (error_identification != FREQ_NORMAL) {      // any error
            if (error_identification == FREQ_FOPEN_ERROR)
                printf("File " Color("%s", YEL) " doesn't exist or you don't have \"r\" permission\n", filename);
            if (error_identification == FREQ_INPUT_ERROR)
                printf(Color("FileRequestCoefficients", YEL) ": " Color("%s", YEL)
                       ": There must be three coefficients like: " Color("1 2 1", MAG) "\n", filename);

            printf("Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");

            fclose(coefficients_file);
            free(filename);

            Answer ask_result = AskYesOrNo();
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                return FREQ_STOP_PROGRAM;
            else
                return FREQ_INPUT_ERROR;
        }
        else /* (error_identification == FREQ_NORMAL) => everything is correct */
            break;

        fclose(coefficients_file);
        free(filename);
    }

    return FREQ_NORMAL;
}


char* GetFilename() {
    char got_symb = 0;
    char* filename = (char*) calloc(MAX_FILENAME_LONG, sizeof(char));
    char* name_end = filename;
    bool clear = true;

    while (1) {                                                                          // request answer unless it is correct
        name_end = filename;
        clear = true;
        while (((got_symb = getchar()) == ' ') || (got_symb == '\t'));                   // skip spaces

        if (!isalpha(got_symb)) {                                                        // first symbol is not letter
            if ((got_symb == '\n') || (got_symb == EOF))
                clear = false;
            goto CLEAR_BUF_AND_REPRINT;                                                  /* continue with clear and reprint */
        }

        *name_end++ = got_symb;                                                          // read first symbol which is letter
        while (isalnum(got_symb = getchar()) || (got_symb == '.') || (got_symb == '_'))  // read symbols until there is space symbol or end of buffer
            *name_end++ = got_symb;

        while ((got_symb == ' ') || (got_symb == '\t'))                                  // skip spaces
            got_symb = getchar();

        if ((got_symb == '\n') || (got_symb == EOF)) {                                   // this is correct name => return it
            *name_end++ = '\0';
            return filename;
        }

        CLEAR_BUF_AND_REPRINT: /* from goto */

        if (clear)
            ClearInputBuf();
        printf("Enter a filename like: " Color("coefficients.txt", YEL) "\n");
    }
}


Req_err RequestCoefficients(double* const p_coef_a, double* const p_coef_b, double* const p_coef_c) {    //TODO any parameters count
    assert(p_coef_a != NULL);
    assert(p_coef_b != NULL);
    assert(p_coef_c != NULL);
    assert(p_coef_a != p_coef_b && p_coef_a != p_coef_c && p_coef_b != p_coef_c);

    while (1) {                          // request the introduction of coefficients unless input is correct or program is closed
        printf("Enter the coefficients coef_a, coef_b, coef_c like: " Color("1 2 1", MAG) "\n");

        if (scanf("%lf %lf %lf", p_coef_a, p_coef_b, p_coef_c) != 3) {
            ClearInputBuf();        // clear the input buffer from excess symbols

            printf(Color("RequestCoefficients:", YEL) "There must be three coefficients like: "
                   Color("1 2 1", MAG) "\n");
            printf("Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");

            Answer ask_result = AskYesOrNo();
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                return REQ_STOP_PROGRAM;
            else
                return REQ_INPUT_ERROR;

        }
        else /* number of arguments is 3 => input is correct */
            break;
    }

    return REQ_NORMAL;
}


Answer AskYesOrNo() {
    char got_symb = 0;
    Answer result = NONE;
    bool clear = true;

    while (1) {                                                                // request answer unless it is correct
        result = NONE;
        clear = true;
        while (((got_symb = getchar()) == ' ') || (got_symb == '\t'));         // skip spaces

        if (got_symb == 'Y')
            result = YES;
        else if (got_symb == 'N')
            result = NO;
        else if ((got_symb == '\n') || (got_symb == EOF))
            clear = false;                                                     // buffer is empty
        if (result != NONE) {
            while (((got_symb = getchar()) == ' ') || (got_symb == '\t'));     // skip spaces
            if ((got_symb == '\n') || (got_symb == EOF))
                return result;
        }

        if (clear)
            ClearInputBuf();

        printf("PLease write " Color("Y", MAG) " or " Color("N", MAG) "\n");
    }
}


void ClearInputBuf(){
    char c = 0;
    while(((c = getchar()) != EOF) && (c != '\n'));
}
