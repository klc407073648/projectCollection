-- auto-generated definition
create table user
(
    username     varchar(256)                       null comment '用户昵称',
    id           bigint auto_increment comment 'id' primary key,
    userAccount  varchar(256)                       null comment '账号',
    avatarUrl    varchar(1024)                      null comment '用户头像',
    gender       tinyint                            null comment '性别',
    userPassword varchar(512)                       not null comment '密码',
    iphone       varchar(128)                       null comment '电话',
    email        varchar(512)                       not null comment '邮箱',
    userStatus   int      default 0                 null comment '状态-0正常',
    createTime   datetime default CURRENT_TIMESTAMP null comment '创建时间',
    updateTime   datetime default CURRENT_TIMESTAMP null on update CURRENT_TIMESTAMP,
    idDelete     tinyint  default 0                 not null comment '是否删除',
    userRole     int      default 0                 not null comment '用户角色 0-普通用户 1-管理员',
    planetCode   varchar(512)                       null comment '星球编号'
)
    comment '用户' ;


INSERT INTO `testuser`.`user` (`username`, `id`, `userAccount`, `avatarUrl`, `gender`, `userPassword`, `iphone`, `email`, `userStatus`, `createTime`, `updateTime`, `idDelete`, `userRole`, `planetCode`) VALUES (NULL, 1, '11111', '11111', NULL, '11111', NULL, '11111', 0, '2022-05-29 05:00:06', '2022-05-29 05:00:06', 0, 0, NULL);
INSERT INTO `testuser`.`user` (`username`, `id`, `userAccount`, `avatarUrl`, `gender`, `userPassword`, `iphone`, `email`, `userStatus`, `createTime`, `updateTime`, `idDelete`, `userRole`, `planetCode`) VALUES (NULL, 2, '22222', '22222', NULL, '22222', NULL, '22222', 0, '2022-05-29 05:00:06', '2022-05-29 05:00:06', 0, 0, NULL);
INSERT INTO `testuser`.`user` (`username`, `id`, `userAccount`, `avatarUrl`, `gender`, `userPassword`, `iphone`, `email`, `userStatus`, `createTime`, `updateTime`, `idDelete`, `userRole`, `planetCode`) VALUES (NULL, 3, '33333', '33333', NULL, '33333', NULL, '33333', 0, '2022-05-29 05:00:06', '2022-05-29 05:00:06', 0, 0, NULL);

alter table user add column tags varchar(1024) null comment '标签列表';
alter table user change tags tags varchar(1024) character set utf8; 

-- auto-generated definition
create table tag
(
    id         bigint auto_increment comment 'id' primary key,
    tagName    varchar(256)                       null comment '标签名称',
    userId     bigint                             null comment '用户ID',
    parentId   bigint                             null comment '父标签ID',
    isParent   tinyint  default 0                 not null comment '是否为父标签 0-不是 1-是',
    createTime datetime default CURRENT_TIMESTAMP null comment '创建时间',
    updateTime datetime default CURRENT_TIMESTAMP null on update CURRENT_TIMESTAMP,
    idDelete   tinyint  default 0                 not null comment '是否删除',
    constraint uniIdx_tagName
        unique (tagName)
)
    comment '标签';

create index idx_userId
    on tag (userId);



