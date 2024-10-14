# Set initial variable values
TEMPLATE="../test_grep/grep_pattern.txt"
PATTERN="../test_grep/grep_pattern.txt" # Path to the pattern file 
TEMPLATE_FILE="../test_grep/grep_test.txt" # Path to the parameter file 
TEST_FILE="../test_grep/grep_test.txt" # Path to the test file
S21="./s21_grep" # Executable
GREP="grep.txt" # File for logs of factory grep
S21_GREP="s21_grep.txt" # File for logs of the tested grep
LOG="grep_log.txt" # File for logs

SUCCESS=0
FAIL=0

# Clear the log file
echo "" > $LOG

# Display information about the files for testing
echo Testing file: $TEST_FILE
echo Template file: $TEMPLATE_FILE

# Function to check command success
check_command_success() {
    if [ $? -eq 0 ]; then
        return 0 # Success
    else
        echo "Test FAILED (Command execution error)"
        return 1 # Failure
    fi
}

# Testing options for grep and s21_grep on the first file
for var in -e -i -v -c -l -n -h -s -o -f
do
    echo "Testing option: $var"
    
    # Run s21_grep with the current option and save the output to the s21_grep.txt file
    $S21 $var $TEMPLATE $TEMPLATE_FILE  > $S21_GREP 
    
    # Check the exit status of the $S21 command
    if check_command_success; then
        # Run grep with the current option and save the output to the grep.txt file
        grep $var $TEMPLATE $TEMPLATE_FILE > $GREP 

        # Compare the s21_grep.txt and grep.txt files. If they are identical, the test is successful
        if cmp -s $S21_GREP $GREP; then
            (( SUCCESS++ ))
            echo "Test SUCCESS"
        else
            # If not, add the current option to the log file and increase the count of failed tests
            echo "Test FAILED"
            echo "$var" >> $LOG
            echo "$var" 
            (( FAIL++ ))
        fi
    else
        # If the command failed, add the current option to the log file and increase the count of failed tests
        (( FAIL++ ))
    fi

    # Remove temporary files s21_grep.txt and grep.txt
    rm $S21_GREP $GREP
done

# Testing options for grep and s21_grep on the second file
for var in -e -i -v -c -l -h -s -o -n -f
do
    echo "Testing option: $var"
    
    # Run s21_grep with the current option and save the output to the s21_grep.txt file
    $S21 $var $TEMPLATE $TEMPLATE_FILE ../test_grep/grep_test_2.txt > $S21_GREP 
    
    # Check the exit status of the $S21 command
    if check_command_success; then
        # Run grep with the current option and save the output to the grep.txt file
        grep $var $TEMPLATE $TEMPLATE_FILE ../test_grep/grep_test_2.txt > $GREP 

        # Compare the s21_grep.txt and grep.txt files. If they are identical, the test is successful
        if cmp -s $S21_GREP $GREP; then
            (( SUCCESS++ ))
            echo "Test SUCCESS"
        else
            # If not, add the current option to the log file and increase the count of failed tests
            echo "Test FAILED"
            echo "$var" >> $LOG
            echo "$var" 
            (( FAIL++ ))
        fi
    else
        # If the command failed, add the current option to the log file and increase the count of failed tests
        (( FAIL++ ))
    fi

    # Remove temporary files s21_grep.txt and grep.txt
    rm $S21_GREP $GREP
done

# Loop for a set of options for search using the $S21 command
for var in -c -ce -ch -cn -co -cs -he -ho -hs -ic -ie -ih -il -in -io -is -iv -l -le -lh -ln -lo -ls -n -ne
do
    echo "Testing option: $var"
    
    # Run the $S21 command with the current option and write the result to the $S21_GREP file
    $S21 $var $TEMPLATE $TEMPLATE_FILE  > $S21_GREP 
    
    # Check the exit status of the $S21 command
    if check_command_success; then
        # Search using grep with the current option and write the result to the $GREP file
        grep $var $TEMPLATE $TEMPLATE_FILE > $GREP 
        
        # Compare the files $S21_GREP and $GREP. If they are identical, increase the count of successful tests
        if cmp -s $S21_GREP $GREP; then
            (( SUCCESS++ ))
            echo "Test SUCCESS"
        else
            # If the files are not identical, write the option to the $LOG file and increase the count of failed tests
            # Print failed tests to the screen for convenience
            echo "Test FAILED"
            echo "$var" >> $LOG
            echo "$var" 
            (( FAIL++ ))
        fi
    else
        # If the command failed, write the option to the $LOG file and increase the count of failed tests
        # Print failed tests to the screen for convenience
        echo "Test FAILED"
        echo "$var" >> $LOG
        echo "$var" 
        (( FAIL++ ))
    fi

    # Remove temporary files
    rm $S21_GREP $GREP
done

# Loop for a set of options for search using the $S21 command
for var in -f -cf -hf -lf -nf -nh -no -ns -o -oe -se -v -vc -ve -vh -vl -vn -vs
do
    echo "Testing option: $var"
    
    # Run the $S21 command with the current option and write the result to the $S21_GREP file
    $S21 $var $PATTERN $TEMPLATE_FILE  > $S21_GREP 
    
    # Check the exit status of the $S21 command
    if check_command_success; then
        # Search using grep with the current option and write the result to the $GREP file 
        grep $var $PATTERN $TEMPLATE_FILE > $GREP 
        
        # Compare the files $S21_GREP and $GREP. If they are identical, increase the count of successful tests
        if cmp -s $S21_GREP $GREP; then
            (( SUCCESS++ ))
            echo "Test SUCCESS"
        else
            # If the files are not identical, write the option to the $LOG file and increase the count of failed tests
            echo "Test FAILED"
            echo "$var" >> $LOG
            echo "$var" 
            (( FAIL++ ))
        fi
    else
        # If the command failed, write the option to the $LOG file and increase the count of failed tests
        echo "Test FAILED"
        echo "$var" >> $LOG
        echo "$var" 
        (( FAIL++ ))
    fi

    # Remove temporary files
    rm $S21_GREP $GREP
done

# Search for lines containing the template and "who" in the template file, output to a file
grep -e $TEMPLATE -e who $TEMPLATE_FILE > $GREP
# Similar search using the $S21 program, output to a file
$S21 -e $TEMPLATE -e who $TEMPLATE_FILE > $S21_GREP
# If the contents of the $S21_GREP and $GREP files are identical, increase the value of the SUCCESS variable by 1, otherwise increase it by 1 for the FAIL variable
if cmp -s $S21_GREP $GREP; then
    (( SUCCESS++ ))
else
    (( FAIL++ ))
fi
# Remove created temporary files
rm $S21_GREP $GREP

# Search for lines matching the pattern in the test file, output to a file
grep -f $PATTERN -e tyt $TEST_FILE > $GREP
# Similar search using the $S21 program, output to a file
$S21 -f $PATTERN -e tyt $TEST_FILE > $S21_GREP 
# If the contents of the $S21_GREP and $GREP files are identical, increase the value of the SUCCESS variable by 1, otherwise increase it by 1 for the FAIL variable, and write "-f -e" to the $LOG file
if cmp -s $S21_GREP $GREP; then
    (( SUCCESS++ ))
else
    echo "-f -e" >> $LOG
    (( FAIL++ ))
fi

# Search for lines containing the template in the test file and s21_grep.h file, output to a file
grep -h $TEMPLATE $TEST_FILE s21_grep.h > $GREP
# Similar search using the $S21 program, output to a file
$S21 -h $TEMPLATE $TEST_FILE s21_grep.h > $S21_GREP
# If the contents of the $S21_GREP and $GREP files are identical, increase the value of the SUCCESS variable by 1, otherwise increase it by 1 for the FAIL variable, and write "-h" to the $LOG file
if cmp -s $S21_GREP $GREP; then
    (( SUCCESS++ ))
else
    echo "-h" >> $LOG
    (( FAIL++ ))
fi

# Remove created temporary files
rm $GREP $S21_GREP

# If the value of the FAIL variable is not equal to 0, display the number of successful and unsuccessful tests, exit the script with code 9, otherwise remove the $LOG file and display the number of successful and unsuccessful tests
if [ $FAIL != 0 ]; then
    echo ""
    echo "SUCCESS: $SUCCESS"
    echo "FAIL: $FAIL"
    exit 9
else
    echo ""
    rm $LOG
    echo "SUCCESS: $SUCCESS"
    echo "FAIL: $FAIL"
fi
