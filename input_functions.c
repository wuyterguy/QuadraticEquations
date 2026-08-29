get_err_t GetCoefficients(Quadratic_coefficients* coef, const Flags* command_line_flags) {
    assert(coef != NULL);
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

    sprintf(output_str, "Quadratic equations solver by " Color("Zotov Anton", YEL) "\n");
    PrintAnimated(output_str, command_line_flags);

    Input_type_t input_type = CONSOLE_INPUT;

    if (command_line_flags->file_input_flag & !command_line_flags->console_input_flag)
        input_type = FILE_INPUT;
    else if (command_line_flags->console_input_flag & !command_line_flags->file_input_flag)
        input_type = CONSOLE_INPUT;
    else
        input_type = AskInputType(command_line_flags);

    if (input_type == FILE_INPUT)
        (void)FileRequestCoefficients(coef, command_line_flags);
    else if (input_type == CONSOLE_INPUT)
        (void)RequestCoefficients(coef, command_line_flags);
    else
        exit(GET_INPUT_ERROR);

    return GET_NORMAL;
}


Input_type_t AskInputType(const Flags* command_line_flags) {
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

    sprintf(output_str, "Would you like to use file input from file? (write " Color("Y", MAG)
           " or " Color("N", MAG) ")\n"
           "If you write N it will be console input\n");
    PrintAnimated(output_str, command_line_flags);

    Answer_t ask_result = AskYesOrNo(command_line_flags);
    if (ask_result == YES)
        return FILE_INPUT;
    else if (ask_result == NO)
        return CONSOLE_INPUT;
    else {
        sprintf(output_str, "End of input file\n");
        PrintAnimated(output_str, command_line_flags);
        return TYPE_ERROR;
    }
}


Freq_err_t FileRequestCoefficients(Quadratic_coefficients* coef, const Flags* command_line_flags) {
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

    assert(coef != NULL);
    assert(&coef->a != NULL);
    assert(&coef->b != NULL);
    assert(&coef->c != NULL);
    assert(&coef->a != &coef->b && &coef->a != &coef->c && &coef->b != &coef->c);

    Freq_err_t error_type = FREQ_NORMAL;

    while (1) {                                          // request the correct file unless input is possible to read 3 coefficients or program is closed
        sprintf(output_str, "Enter a filename like: " Color(DEFAULT_FILE, YEL)
               " (press " Color("Enter", MAG) " to use default file)" "\n");
        PrintAnimated(output_str, command_line_flags);

        char* filename = GetFilename(command_line_flags);
        if (filename == NULL) {
            sprintf(output_str, "End of input file\n");
            PrintAnimated(output_str, command_line_flags);
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
                sprintf(output_str, "File " Color("%s", YEL) " - error during the reading\n", filename);
                PrintAnimated(output_str, command_line_flags);

                fclose(coefficients_file);
                free(filename);
                exit(FREQ_READ_ERROR);
            }
        }

        if (error_type != FREQ_NORMAL) {      // any error
            if (error_type == FREQ_FOPEN_ERROR) {
                sprintf(output_str, "File " Color("%s", YEL) " doesn't exist or you don't have \"r\" permission\n", filename);
                PrintAnimated(output_str, command_line_flags);
            }
            if (error_type == FREQ_INPUT_ERROR) {
                sprintf(output_str, Color("FileRequestCoefficients", YEL) ": " Color("%s", YEL)
                       ": There must be three coefficients like: " Color("1 5 6", MAG) "\n", filename);
                PrintAnimated(output_str, command_line_flags);
            }

            sprintf(output_str, "Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");
            PrintAnimated(output_str, command_line_flags);

            fclose(coefficients_file);
            free(filename);

            Answer_t ask_result = AskYesOrNo(command_line_flags);
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                exit(FREQ_STOP_PROGRAM);
            else {
                sprintf(output_str, "End of input file\n");
                PrintAnimated(output_str, command_line_flags);
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


char* GetFilename(const Flags* command_line_flags) {
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

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
        sprintf(output_str, "Enter a filename like: " Color(DEFAULT_FILE, YEL) "\n");
        PrintAnimated(output_str, command_line_flags);
    }
}


Req_err_t RequestCoefficients(Quadratic_coefficients* coef, const Flags* command_line_flags) {
    assert(coef != NULL);
    assert(command_line_flags != NULL);
    assert(&coef->a != NULL);
    assert(&coef->b != NULL);
    assert(&coef->c != NULL);
    assert(&coef->a != &coef->b && &coef->a != &coef->c && &coef->b != &coef->c);

    char output_str[MAX_OUTOUT_LENGTH];

    int first_symb = 0;

    while (1) {                          // request the introduction of coefficients unless input is correct or program is closed
        sprintf(output_str, "Enter the coefficients coef_a, coef_b, coef_c like: " Color("1 5 6", MAG)
               " (press " Color("Enter", MAG) " to use coefficients from example)" "\n");
        PrintAnimated(output_str, command_line_flags);

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

            sprintf(output_str, Color("RequestCoefficients:", YEL) "There must be three coefficients like: "
                   Color("1 2 1", MAG) "\n"
                   "Do you want to try again? (write " Color("Y", MAG) " if you do or "
                   Color("N", MAG) " otherwise)\n");
            PrintAnimated(output_str, command_line_flags);

            Answer_t ask_result = AskYesOrNo(command_line_flags);
            if (ask_result == YES)
                continue;
            else if (ask_result == NO)
                exit(REQ_STOP_PROGRAM);
            else {
                sprintf(output_str, "End of input file\n");
                PrintAnimated(output_str, command_line_flags);
                exit(REQ_INPUT_ERROR);
            }

        }
        else { /* number of arguments is 3 => input is correct */
            ClearInputBuf();
            break;
        }
    }

    return REQ_NORMAL;
}


Answer_t AskYesOrNo(const Flags* command_line_flags) {
    assert(command_line_flags != NULL);

    char output_str[MAX_OUTOUT_LENGTH];

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

        sprintf(output_str, "PLease write " Color("Y", MAG) " or " Color("N", MAG) "\n");
        PrintAnimated(output_str, command_line_flags);
    }
}


void ClearInputBuf(){
    int c = 0;
    while(((c = getchar()) != EOF) && (c != '\n'));
}
