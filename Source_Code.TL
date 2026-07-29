start

# 1. Create a 1D array representing the 3x3 board
board = [" ", " ", " ", " ", " ", " ", " ", " ", " "]
gameRunning = 1
turn = "X"

# 2. Main Game Loop
loop(rounds, 0, 9, 1)
    
    check (gameRunning == 1)
        # Display the board layout properly row by row
        out(board[0]) out(" | ") out(board[1]) out(" | ") out(board[2])
        out("\n")
        out("-----------")
        out("\n")
        out(board[3]) out(" | ") out(board[4]) out(" | ") out(board[5])
        out("\n")
        out("-----------")
        out("\n")
        out(board[6]) out(" | ") out(board[7]) out(" | ") out(board[8])
        out("\n")

        out("Player turn:")
        out(turn)
        out("\n")
        
        out("Enter index (0-8):")
        in(choice)

        # Update board if the cell is empty
        check (board[choice] == " ")
            board[choice] = turn
            
            # --- CHECK ALL WINNING CONDITIONS ---
            # Rows
            check (board[0] == turn and board[1] == turn and board[2] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end
            check (board[3] == turn and board[4] == turn and board[5] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end
            check (board[6] == turn and board[7] == turn and board[8] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end

            # Columns
            check (board[0] == turn and board[3] == turn and board[6] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end
            check (board[1] == turn and board[4] == turn and board[7] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end
            check (board[2] == turn and board[5] == turn and board[8] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end

            # Diagonals
            check (board[0] == turn and board[4] == turn and board[8] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end
            check (board[2] == turn and board[4] == turn and board[6] == turn)
                out("Player ")
                out(turn)
                out(" Wins!")
                out("\n")
                gameRunning = 0
            end
            # -----------------------------------

            # Swap Turns (only if game is still running)
            check (gameRunning == 1)
                check(turn == "X") turn = "O"
                else turn = "X"
                end
            end
        else
            out("Spot taken! Try again.")
            out("\n")
            # Rewind loop round step to retry turn
            rounds = rounds - 1
        end
    end

end

out("Game Over!")

end