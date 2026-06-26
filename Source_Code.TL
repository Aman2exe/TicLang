start

# 1. Create a 1D array representing the 3x3 board
board = [" ", " ", " ", " ", " ", " ", " ", " ", " "]
gameRunning = 1
turn = "X"

# 2. Main Game Loop
loop(rounds, 0, 9, 1)
    
    check (gameRunning == 1)
        # Display the board layout
        out(board[0]) out("|") out(board[1]) out("|") out(board[2])
        out(board[3]) out("|") out(board[4]) out("|") out(board[5])
        out(board[6]) out("|") out(board[7]) out("|") out(board[8])

        out("Player turn:")
        out(turn)
        
        out("Enter index (0-8):")
        in(choice)

        # Update board if the cell is empty
        check (board[choice] == " ")
            board[choice] = turn
            
            # Simple check if player won horizontally on top row
            check (board[0] == turn and board[1] == turn and board[2] == turn)
                out("Winner written!")
                gameRunning = 0
            end

            # Swap Turns
            check(turn == "X") turn = "O"
            else turn = "X"
            end
        else
            out("Spot taken! Try again.")
            # Rewind loop round step to retry turn
           rounds = rounds - 1
        end
    end

end

out("Game Over!")

end