#/bin/bash

yaml_file='../conf/config.yml'
json_file='../conf/config.json'

function modify_yaml_file() 
{
    echo "begin to modify_yaml_file"

    sed -i "s/\$MYSQL_HOST_IP/81.68.132.31/g" ${yaml_file}
    sed -i "s/\$MYSQL_HOST_PORT/3308/g" ${yaml_file}
    sed -i "s/\$MYSQL_DB_NAME/hmdp/g" ${yaml_file}
    sed -i "s/\$MYSQL_DB_USER/root/g" ${yaml_file}
    sed -i "s/\$MYSQL_DB_PWD/456789/g" ${yaml_file}
    sed -i "s/\$MYSQL_CON_NUM/1/g" ${yaml_file}

    sed -i "s/\$REDIS_HOST_IP/81.68.132.31/g" ${yaml_file}
    sed -i "s/\$REDIS_HOST_PORT/6380/g" ${yaml_file}
    sed -i "s/\$REDIS_DB_NAME/0/g" ${yaml_file}
    sed -i "s/\$REDIS_DB_USER/root/g" ${yaml_file}
    sed -i "s/\$REDIS_DB_PWD/456789/g" ${yaml_file}
    sed -i "s/\$REDIS_CON_NUM/1/g" ${yaml_file}

    sed -i "s/\$THREADPOOL_MIN_SIZE/2/g" ${yaml_file}
    sed -i "s/\$THREADPOOL_MAX_SIZE/16/g" ${yaml_file}
    sed -i "s/\$THREADPOOL_IDLE_TIME/60/g" ${yaml_file}

    echo "end to modify_yaml_file"
}

function modify_json_file() 
{
    echo "begin to modify_json_file"

    sed -i "s/\$MYSQL_HOST_IP/81.68.132.31/g" ${json_file}
    sed -i "s/\$MYSQL_HOST_PORT/3308/g" ${json_file}
    sed -i "s/\$MYSQL_DB_NAME/hmdp/g" ${json_file}
    sed -i "s/\$MYSQL_DB_USER/root/g" ${json_file}
    sed -i "s/\$MYSQL_DB_PWD/456789/g" ${json_file}
    sed -i "s/\$MYSQL_CON_NUM/1/g" ${json_file}

    echo "end to modify_json_file"
}

modify_yaml_file
modify_json_file
