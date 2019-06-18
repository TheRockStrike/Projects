# Go to the Desktop for easier accessibility
cd C:\Users\Administrator\Desktop

# Make a dummy directory to show it worked
mkdir Test

# Move into that directory
cd Test

# Create the SSH private key and store it here
aws ec2 create-key-pair --key-name MyKeyPair --query 'KeyMaterial' --output text | out-file -encod
ing ascii -filepath private_key.pem