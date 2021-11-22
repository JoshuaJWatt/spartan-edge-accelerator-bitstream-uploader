
<?php
    $currentDirectory = getcwd();
    $uploadDirectory = "/bitstreams/";

    $errors = []; // Store errors here

    $fileExtensionsAllowed = ['bit']; // These will be the only file extensions allowed 

    $fileName = $_FILES['thebitstream']['bitstream'];
    $fileSize = $_FILES['thebitstream']['size'];
    $fileTmpName  = $_FILES['thebitstream']['tmp_name'];
    $fileType = $_FILES['thebitstream']['type'];
    $fileExtension = strtolower(end(explode('.',$fileName)));

    $uploadPath = $currentDirectory . $uploadDirectory . basename($fileName); 

    if (isset($_POST['submit'])) {

      if (! in_array($fileExtension,$fileExtensionsAllowed)) {
        $errors[] = "This file extension is not allowed. Please upload a bit file";
      }

      if ($fileSize > 4000000) {
        $errors[] = "File exceeds maximum size (4MB)";
      }

      if (empty($errors)) {
        $didUpload = move_uploaded_file($fileTmpName, $uploadPath);

        if ($didUpload) {
          echo "The file " . basename($fileName) . " has been uploaded";
        } else {
          echo "An error occurred. Please contact the administrator.";
        }
      } else {
        foreach ($errors as $error) {
          echo $error . "These are the errors" . "\n";
        }
      }

    }
?>