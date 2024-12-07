import turtle
import math
import time

def draw_graph(input_file, output_file):
    # Read data from the input file
    with open(input_file, "r") as file:
        lines = file.read().strip().split("\n")
    
    # Extract the number of cities and roads
    num_cities, num_roads = map(int, lines[0].split())
    road_data = [tuple(map(int, line.split())) for line in lines[1:-1]]
    special_cities = tuple(map(int, lines[-1].split()))

    # Read the special path from the output file
    with open(output_file, "r") as file:
        special_path = list(map(int, file.read().strip().split()))

    # Set up the screen and expand it to fit the entire drawing
    screen = turtle.Screen()
    screen.bgcolor("white")
    screen.title("Graph Visualization")
    screen.setup(width=screen.window_width(), height=screen.window_height())  # Open the screen fully

    # Set up the turtle for drawing
    graph_turtle = turtle.Turtle()
    graph_turtle.speed(0)
    graph_turtle.penup()

    # Calculate the positions of cities in a circular arrangement
    radius = 30 * num_cities
    radius = min(radius,360)
    positions = {}
    angle_step = 360 / num_cities
    for city in range(1, num_cities + 1):
        angle = math.radians(city * angle_step)
        x = radius * math.cos(angle)
        y = radius * math.sin(angle)
        positions[city] = (x, y)

    # Draw the cities
    for city, (x, y) in positions.items():
        graph_turtle.goto(x, y)
        if city == special_cities[0]:  # The first special city (in red)
            graph_turtle.dot(20, "#FF6347")  # Tomato Red
        elif city == special_cities[1]:  # The second special city (in green)
            graph_turtle.dot(20, "#32CD32")  # Lime Green
        else:  # Other cities in blue
            graph_turtle.dot(20, "#1E90FF")  # Dodger Blue
        #graph_turtle.write(f" {city}", font=("Arial", 25, "bold"))
        time.sleep(0.2)  # Pause to draw each city

    radius +=40
    nums_positions = {}
    angle_step = 360 / num_cities
    for city in range(1, num_cities + 1):
        angle = math.radians(city * angle_step)
        x = radius * math.cos(angle)
        y = radius * math.sin(angle)
        nums_positions[city] = (x, y)
        
    shift_x = 0 
    shift_y= 0
    for city, (x, y) in nums_positions.items():
        graph_turtle.goto(x - shift_x, y - shift_y)
        shift_y+=2
        shift_x+=2
        graph_turtle.write(f" {city}", font=("Arial", 25, "bold"))
        time.sleep(0.2)  # Pause to draw each city



    # Draw the normal roads
    graph_turtle.pensize(2)
    graph_turtle.pencolor("#A9A9A9")  # Dark Gray for normal roads
    for road in road_data:
        city1, city2 = road
        x1, y1 = positions[city1]
        x2, y2 = positions[city2]

        # Draw the road with a quick speed
        graph_turtle.penup()
        graph_turtle.goto(x1, y1)
        graph_turtle.pendown()
        steps = 20  # Number of steps
        for i in range(steps + 1):
            graph_turtle.goto(
                x1 + (x2 - x1) * i / steps,
                y1 + (y2 - y1) * i / steps
            )
            time.sleep(0.005)  # Speed of drawing the road
        time.sleep(0.1)  # Short pause after each road

    # Draw the special roads with a wider line and arrows
    graph_turtle.pensize(4)  # Wider line for special roads
    graph_turtle.pencolor("#FFA500")  # Orange for special roads
    if -1 not in special_path:  # Only if there is no -1 in the special path
        for i in range(len(special_path) - 1):
            city1 = special_path[i]
            city2 = special_path[i + 1]
            x1, y1 = positions[city1]
            x2, y2 = positions[city2]

            # Draw the special road with arrows
            graph_turtle.penup()
            graph_turtle.goto(x1, y1)
            graph_turtle.pendown()
            steps = 20  # Number of steps
            for j in range(steps + 1):
                graph_turtle.goto(
                    x1 + (x2 - x1) * j / steps,
                    y1 + (y2 - y1) * j / steps
                )
                time.sleep(0.005)  # Speed of drawing the special road

            # Drawing the arrow
            graph_turtle.penup()
            graph_turtle.goto(x2, y2)
            graph_turtle.setheading(graph_turtle.towards(x1, y1))
            graph_turtle.pendown()
            graph_turtle.forward(10)
            graph_turtle.right(150)
            graph_turtle.forward(15)
            graph_turtle.penup()
            graph_turtle.backward(15)
            graph_turtle.left(300)
            graph_turtle.pendown()
            graph_turtle.forward(15)

            time.sleep(0.1)  # Short pause after each special road

    # Add the fixed text: "This is the best Path"
    if -1 not in special_path:
        graph_turtle.hideturtle()  # Hide the turtle temporarily
        message_turtle = turtle.Turtle()
        message_turtle.hideturtle()
        message_turtle.speed(2)
        message_turtle.penup()
        message_turtle.goto(0, radius + 50)  # Position the message in the circle (center of the screen)

        # Write the message inside the circle
        message_turtle.pendown()
        message_turtle.pencolor("#8A2BE2")  # Blue Violet for the message
        message_turtle.write("This is the best Path", font=("Arial", 24, "bold"), align="center")
    else:
        # If there is -1, display the appropriate message
        message_turtle = turtle.Turtle()
        message_turtle.hideturtle()
        message_turtle.penup()
        message_turtle.goto(0, radius + 50)
        message_turtle.write("You can't reach this city", font=("Arial", 24, "bold"), align="center")

    
    # Finish drawing
    turtle.done()

# Input and output file names
input_file = "input.txt"
output_file = "output.txt"

# Run the program
draw_graph(input_file, output_file)
