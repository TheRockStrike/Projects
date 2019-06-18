$key='-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEA88R2Q0AVQ0F5EqaN1qIiAiPHRKI2lCRwwpmDZofxqpbjiNt7
G+xadkAYaX+BY02qnC6GgWAoRRMmRh/6VdzISaR9GeAVedWUAsCLEceO9FnGWF27
7wfx78/+tpUKqPr/Apl5VkmXUQdCqrYyBUzGtX5vaEulGaqWMqtDk/tS5bU9gkE9
tpwzufdFTDGOhFVQ2NueUhliSkOCQLQVUIfUXyONaMAMG85yDTbuISqCqvdq4o1Q
m7+sA72GubNm0P4YqbBGYveg6nazPkmyKD/wsRHLWRexxCJxEjb4EqDUAJIGwV2Q
zeN7Czucz9nXK+7mDS8ivH1d535d7OOM1X+uIwIBIwKCAQEA1+iGAQzfofgpaEpg
XwShx2GEm+AhtmlrMARPz9d+RqLmyax0Wo+DUssOTtAE4u0E03mqVVUq+1oaleHP
GNl+MpkBFusLtQ2gWjWCfXY8ycvbkBiB4ld+g/K9CB2buj1lhfWl+/gJrisAiJK+
lvrb4pSOj5N0+XnVfVXcxOXqVmuZLUJF0DYqyXjDtaTb8mPFIcmBMJ9OHVqycMjo
gQ2jKOhNw7oRFluMFEBs+HcBx2soywxaW1QNezin9TWjcaVeesP3aawrbqJwJuB4
hqwvEsqKrlLS48PvL4SWNNxn7WBbeQmJ0RtVW8bt6x+rdHGkk6d9jYzRwaLV6GM6
ApXkswKBgQD9PAfS81Xrt16JgjSV7KSCEUpKwue5pqGY+ffWAg0hS2TwaXL6q0cZ
dqNuURcc+hNY74t7Ve1uJMnK0R9bH/lCxorJxR22RJLIEYvo7Wn/twaLrIbZv4ji
unZdLb2xQaulm10/5D56ZilW1aDnZcmFdb3BNBNPd+YpsqWRwv1RfQKBgQD2bfbZ
4HR6zIxB7GPfO7PG675WWEw/7dhqIjyDH97qWSvynjCrkRe6bzH8hgClvzmJDcJW
e5yiQ1CrsrJTKHNrN4IKcN25nJfgKiNys01Yu4mrW1yfmBcttr+ONZe1qqLhGTec
ifF/PXO9fSWyUzF1Ow8eGiXnzseVTKSpD5cQHwKBgA54dXnEwxTIplEAIEMU1jNR
cfWkvMjHsXZ0rxOLFrFyBcSYT7aNccbw1iOPmuu2hMNA42YiKtMYC4fusVWqDj5U
fPWWPDZND7OpOzHhrkkgZsYnHaYK8eER+CKU5kSkqrkBkEzLNsUqaMMiJnOfapnp
eI6yhMK2ZOxwm78vtrQzAoGADhTpiMr/V3lnGbW8kGnPwjlbVWQhnUDKibjQQgHS
OUbskYVh3esIq5FTUEIr7DbQFnXQlzpDdvyIRFNapaqKQKQWDzmmRR7kG3AfSGIE
a3huRE5dEG8XRHDXvvu/hrlLIs49sSx0MygVPgcmubucbRlRUi1hQHI3SlwmqpMt
NB8CgYEAvXjzWDC3WZEOkeWvG6pli/8DIaO+P4eAeLiS4uAc7Ed+JLTRYObq+IZi
PQCBKUXkF+KpR05mQNDi089+jIN6KobZQNGQlFblVREX49Ve8LNIQiteJAhJEkQW
ppnx7eb+PywA0tyxnruhTy3Ee6rP4hz0nBHG4P5+rvkg43OF3JU=
-----END RSA PRIVATE KEY-----
'

# Go to home
cd C:\Users\Administrator

# Make .ssh folder
mkdir .ssh

# Move into that directory
cd .ssh

# Put
${key} | Out-File -FilePath .\id_rsa 
