;;; sdfeclient.el --- sdfEngine Client -*- lexical-binding: t; -*-
;;
;; Copyright (C) 2023 wwotz
;;
;; Author: wwotz <wwotz12@gmail.com>
;; Maintainer: wwotz <wwotz12@gmail.com>
;; Created: April 04, 2023
;; Modified: April 04, 2023
;; Version: 0.0.1
;; Keywords: Symbol’s value as variable is void: finder-known-keywords
;; Homepage: https://github.com/wwotz/sdfEngine
;; Package-Requires: ((emacs "24.3"))
;;
;; This file is not part of GNU Emacs.
;;
;;; Commentary:
;;
;;  Description
;;
;;; Code:

(defvar sdfe-server-service 3490)     ; port of the server
(defvar sdfe-server-host "127.0.0.1") ; localhost
(defvar sdfe-server-process nil) ; the server process
(defvar sdfe-server-bufsize 256) ; the server's buffer size
(defvar sdfe-server-shader-refresh-command "<shader-refresh>") ; refresh command for the sdfe-server.
(defvar sdfe-server-timer-reset-command "<timer-reset>") ; timer reset for the sdfe-server.

(defun sdfe-buffer-absolute-path nil
  "returns the absolute path of the current buffer."
  (interactive)
  (substring (concat (pwd) (buffer-name)) 10))

(defun sdfe-client-send-shader nil
  "sends the current buffers fragment shader path to the sdfe-server."
  (interactive)
  (if (not (null sdfe-server-process))
      (process-send-string sdfe-server-process (string-pad (get-buffer-absolute-path) sdfe-server-bufsize 0))
    nil))

(defun sdfe-client-timer-reset nil
  "causes the sdf-server to set the time uniform to 0.0."
  (interactive)
  (if (not (null sdfe-server-process))
      (process-send-string sdfe-server-process (string-pad sdfe-server-timer-reset-command sdfe-server-bufsize 0))))

(defun sdfe-client-shader-refresh nil
  "causes the sdf-server to re-read the fragment shader."
  (interactive)
  (if (not (null sdfe-server-process))
      (process-send-string sdfe-server-process (string-pad sdfe-server-shader-refresh-command sdfe-server-bufsize 0))))

(if (not (null sdfe-server-process))
    (print "sdfe-server-process is not null")
  (print "sdfe-server-process is null"))

(defun sdfe-client-listen-start nil
  "starts listening to the sdfe-server."
  (interactive)
  (if (null sdfe-server-process)
      (setq sdfe-server-process (make-network-process :name "sdfe-client" :buffer "*sdfe-client*"
                                                       :family 'ipv4 :host sdfe-server-host
                                                       :service sdfe-server-service
                                                       :sentinel 'sdfe-client-listen-sentinel
                                                       :filter 'sdfe-client-listen-filter))
    nil))

(defun sdfe-client-listen-stop nil
  "stops listening to the sdfe-server."
  (interactive)
  (progn
    (delete-process sdfe-server-process)
    (setq sdfe-server-process nil)))

(defun sdfe-client-listen-filter (proc string)
  (message string))

(setq current-buffer-path (get-buffer-absolute-path))

(defun sdfe-client-listen-sentinel (proc string)
  (when (string= msg "connection broken by remote peer\n")
    (progn
      (message (format "client %s has quit" proc))
      (setq sdfe-server-process nil))))

(provide 'sdfe-client)
;;; client.el ends here
