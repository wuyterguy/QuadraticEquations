get_err_t GetCoefficients(Quadratic_coefficients* coef,Flags command_line_flags) {
    assert(coef != NULL);

    printf("Quadratic equations solver by " Color("Zotov Anton", YEL) "\n");

    Input_type_t input_type = CONSOLE_INPUT;

    if (command_line_flags.file_input_flag & !command_line_flags.console_input_flag)
        input_type = FILE_INPUT;
    else if (command_line_flags.console_input_flag & !command_line_flags.file_input_flag)
        input_type = CONSOLE_INPUT;
    else
        input_type = AskInputType();

    if (input_type == FILE_INPUT)
        (void)FileRequestCoefficients(coef);
    else if (input_type == CONSOLE_INPUT)
        (void)RequestCoefficients(coef);
    else
        exit(GET_INPUT_ERROR);

    return GET_NORMAL;
}


Input_type_t AskInputType() {
    printf("Would you like to use file input from file? (write "
           Color("Y", MAG) " or " Color("N", MAG) ")\n");
    printf("If you write N it will be console input\n");

    Answer_t ask_result = AskYesOrNo();
    if (ask_result == YES)
        return FILE_INPUT;
    else if (ask_result == NO)
        return CONSOLE_INPUT;
    else {
        printf("End of input file\n");
        return TYPE_ERROR;
    }
}


Freq_err_t FileRequestCoefficients(Quadratic_coefficients* coef) {
    assert(coef != NULL); // TODO arguments of flags
    assert(&coef->a != NULL); // TODO pass test file
    assert(&coef->b != NULL);
    assert(&coef->c != NULL);
    assert(&coef->a != &coef->b && &coef->a != &coef->c && &coef->b != &coef->c); // TODO collect warnings

    Freq_err_t error_type = FREQ_NORMAL;

    while (1) {                                          // request the correct file unless input is possible to read 3 coefficients or program is closed
        printf("Enter a filename like: " Color(DEFAULT_FILE, YEL)
               " (press " Color("Enter", MAG) " to use default file)" "\n");

        char* filename = GetFilename();
        if (filename == NULL) {
            printf("End of input file\n");
            exit(FREQ_INPUT_ERROR);
        }

        FILE* coefficients_file = fopen(filename, "r");

        error_type = FREQ_NORMAL;

        if (coefficients_file == NULL)
            error_type = FREQ_FOPEN_ERROR;
        else { /* (coefficients_file != NULL) */
            if (fscanf(coefficients_file, "%lf %lf %lf", &coef->a, &coef->b, &coef->c) != 3)
                error_type = FREQ_INPUT_ERROR;

            if (ferror(coefficients_file)) {
                printf("File " Color("%s", YEL) " - error during the reading\n", filename);

                fclose(coefficients_file);
                free(filename);
                exit(FREQ_READ_ERROR);
            }
        }

        if (error_type != FREQ_NORMAL) {      // any error
            if (error_type == FREQ_FOPEN_ERROR)
                printf("File " Color("%s", YEL) " doesn't exist or you don't have \"r\" permission\n", filename);
            if (error_type == FREQ_INPUT_ERROR)
                printf(Color("FileRequestCoefficients", YEL) ": " Color("%s", YEL)
                       ": There must be three coefficients like: " Color("1 5 6", MAG) "\n", filename);

            printf("Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");

            fclose(coefficients_file);
            free(filename);

            Answer_t ask_result = AskYesOrNo();
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                exit(FREQ_STOP_PROGRAM);
            else {
                printf("End of input file\n");
                exit(FREQ_INPUT_ERROR);
            }
        }
        else /* (error_type == FREQ_NORMAL) => everything is correct */
            break;

        fclose(coefficients_file);
        free(filename);
    }

    return FREQ_NORMAL;
}


char* GetFilename() {
    int got_symb = 0;
    char* filename = (char*) calloc(MAX_FILENAME_LENGTH, sizeof(char));
    assert(filename != NULL);
    char* name_end = filename;

    while (1) {                                                                          // request answer unless it is correct
        name_end = filename;
        while (((got_symb = getchar()) == ' ') || (got_symb == '\t'));                   // skip spaces

        if (!isalpha(got_symb)) {                                                        // first symbol is not letter
            if (got_symb == '\n') {                                                      // return default value
                strncpy(filename, DEFAULT_FILE, MAX_FILENAME_LENGTH);
                return filename;
            }
            if (got_symb == EOF)
                return NULL;
        }

        *name_end++ = (char)got_symb;                                                    // read first symbol which is letter
        while (isalnum(got_symb = getchar()) || (got_symb == '.') || (got_symb == '_'))  // read symbols until there is space symbol or end of buffer
            *name_end++ = (char)got_symb;

        while ((got_symb == ' ') || (got_symb == '\t'))                                  // skip spaces
            got_symb = getchar();

        if ((got_symb == '\n') || (got_symb == EOF)) {                                   // this is correct name => return it
            *name_end++ = '\0';
            return filename;
        }

        ClearInputBuf();
        printf("Enter a filename like: " Color(DEFAULT_FILE, YEL) "\n");
    }
}


Req_err_t RequestCoefficients(Quadratic_coefficients* coef) {
    assert(coef != NULL);
    assert(&coef->a != NULL);
    assert(&coef->b != NULL);
    assert(&coef->c != NULL);
    assert(&coef->a != &coef->b && &coef->a != &coef->c && &coef->b != &coef->c);

    int first_symb = 0;

    while (1) {                          // request the introduction of coefficients unless input is correct or program is closed
        printf("Enter the coefficients coef_a, coef_b, coef_c like: " Color("1 5 6", MAG)
               " (press " Color("Enter", MAG) " to use coefficients from example)" "\n");

        if((first_symb = getchar()) == '\n'){
            coef->a = 1;
            coef->b = 5;
            coef->c = 6;
            return REQ_NORMAL;
        }
        else
            ungetc(first_symb, stdin);

        if (scanf("%lf %lf %lf", &coef->a, &coef->b, &coef->c) != 3) {  // TODO think
            ClearInputBuf();             // clear the input buffer from excess symbols

            printf(Color("RequestCoefficients:", YEL) "There must be three coefficients like: "
                   Color("1 2 1", MAG) "\n");
            printf("Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");

            Answer_t ask_result = AskYesOrNo();
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                exit(REQ_STOP_PROGRAM);
            else {
                printf("End of input file\n");
                exit(REQ_INPUT_ERROR);
            }

        }
        else /* number of arguments is 3 => input is correct */
            break;
    }

    return REQ_NORMAL;
}


Answer_t AskYesOrNo() {
    int got_symb = 0;
    Answer_t result = NONE;
    bool clear = true;

    while (1) {                                                                // request answer unless it is correct
        result = NONE;
        clear = true;
        while (((got_symb = getchar()) == ' ') || (got_symb == '\t'));         // skip spaces

        if (got_symb == 'Y')
            result = YES;
        else if (got_symb == 'N')
            result = NO;
        else if (got_symb == '\n')
            clear = false;                                                     // buffer is empty
        else if (got_symb == EOF)
            return NONE;
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
    int c = 0;
    while(((c = getchar()) != EOF) && (c != '\n'));
}
