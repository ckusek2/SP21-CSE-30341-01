# SP21-CSE-30341-01

Group members:

  Calvin Kusek- ckusek2@nd.edu
  Rafael Mendizabal- rmendiza@nd.edu
  Dalton Dove- ddove@nd.edu
  
For both textdetect and dtextdetect, we are handling the files that contain zero bytes by outputting NaN as the percentage, as it is not an error.

In dtextdetect, the results.json file is overwritten every time the program is run. This allows us to not have to check if there is already a results.json file present.

In dtextdetect, we make sure the directory is valid at the very beginning of the program and then we do not have to check to make sure the files are valid because we are already looping over known files in the directory.
