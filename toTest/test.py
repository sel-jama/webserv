#!C:/Users/Gaurav/AppData/Local/Programs/Python/Python310/python.exe
#Importing CGI module
import cgi

def generate_html_header():
	print("Content-type: text/html\r\n\r\n")
	print("<html><head>")
	print("<title>Dynamic Webpage</title>")
	print("<style>")
	print("body { font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; }")
	print(".container { max-width: 500px; margin: 0 auto; background-color: #fff; padding: 20px; border: 1px solid #ddd; border-radius: 5px; }")
	print("form { text-align: center; }")
	print("input[type='text'] { padding: 10px; width: 100%; border: 1px solid #ccc; border-radius: 5px; }")
	print("input[type='submit'] { background-color: #4CAF50; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; }")
	print("h1 { color: green; text-align: center; transform-origin: center bottom; transition: transform 0.3s; }")
	print("h1:hover { transform: rotate(180deg) scale(1.1); }")
	print(".title-container { text-align: center; margin-top: 20px; }")
	print("@keyframes bounce { 0%, 100% { transform: translateY(0); } 50% { transform: translateY(-20px); }}")
	print("</style>")
	print("</head><body>")
	print("<div class='container'>")

# Function to generate the HTML form for square calculation
def generate_html_form():
	print("<form method='post' action=''>")
	print("<p>Enter a number: <input type='text' name='number' /></p>")
	print("<input type='submit' value='Calculate Square' />")
	print("</form>")

# Function to calculate and display the square of a number
def calculate_square():
	form = cgi.FieldStorage()
	number = form.getvalue("number")
	if number:
		try:
			number = float(number)
			square = number ** 2
			print("<p>The square of {} is {:.2f}</p>".format(number, square))
		except ValueError:
			print("<p>Please enter a valid number to calculate its square.</p>")

# Generating the HTML content
generate_html_header()
print("<div class='title-container'><h1>&#x1F60A;</h1></div>")
generate_html_form()

# Adding a section for calculating the square of a number
calculate_square()

# Closing the HTML document
print("</div></body></html>")
